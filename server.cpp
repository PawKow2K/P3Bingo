#include "server.h"
#include "networkmessages.h"

Server::Server(int port, QObject *parent) : QObject(parent)
{
	setPort(port);
	server = new QTcpServer(this);
	connect(server, &QTcpServer::newConnection, this, &Server::newConnection);
}

void Server::newConnection()
{
	while (server->hasPendingConnections())
	{
		QTcpSocket* socket = server->nextPendingConnection();
		connect(socket, &QTcpSocket::disconnected, this, &Server::disconnected);
		connect(socket, &QTcpSocket::readyRead, this, &Server::readyRead);
		clients.insert(socket);

		NetworkMessage init = getInitMessage();

		sendMessage(socket, init);
	}
}

void Server::disconnected()
{
	QTcpSocket* who = qobject_cast<QTcpSocket*>(sender());
	who->deleteLater();
	clients.erase(who);
}

void Server::readyRead()
{
	QTcpSocket* socket = qobject_cast<QTcpSocket*>(sender());
	QDataStream stream(socket);
	stream.startTransaction();
	NetworkMessage msg;
	stream >> msg;

	if (!stream.commitTransaction())
		return;

	if (const UpdateMessage* update = std::get_if<UpdateMessage>(&msg))
	{
		emit stateUpdate(update->x, update->y, update->team);
		broadcastStateUpdate(update->x, update->y, update->team);
	}
}

void Server::sendMessage(QTcpSocket* socket, NetworkMessage& msg)
{
	QDataStream stream(socket);
	stream << msg;
}

void Server::setGame(Game* game)
{
	currentGame = game;
}

int Server::getPort()
{
	return port;
}

void Server::setPort(int newPort)
{
	port = newPort;
}

NetworkMessage Server::getInitMessage()
{
	int width = currentGame->columnCount();
	int height = currentGame->rowCount();
	std::vector<Tile> board = currentGame->getCurrentBoard();
	std::vector<TeamData> teams = currentGame->getTeams();
	return InitMessage{width, height, board, teams};
}

void Server::broadcastNewGame()
{
	NetworkMessage msg = getInitMessage();

	for (auto socket : clients)
	{
		sendMessage(socket, msg);
	}
}

void Server::broadcastStateUpdate(int x, int y, teamid_t team)
{
	NetworkMessage msg = UpdateMessage{x, y, team};

	for (auto socket : clients)
	{
		sendMessage(socket, msg);
	}
}

bool Server::start()
{
	return server->listen(QHostAddress::Any, port);
}

void Server::stop()
{
	for (auto socket : clients)
	{
		socket->disconnectFromHost();
	}
	server->close();
}
