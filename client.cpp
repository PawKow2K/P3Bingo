#include "client.h"
#include "networkmessages.h"

Client::Client(QString ip, int port, QObject *parent) : QObject(parent)
{
	setHost(ip, port);
	socket = new QTcpSocket(this);
	stream.setDevice(socket);
	connect(socket, &QTcpSocket::readyRead, this, &Client::readyRead);
	connect(socket, &QTcpSocket::disconnected, this, &Client::disconnected);
}

void Client::setHost(QString newHostname, int newPort)
{
	hostname = newHostname;
	port = newPort;
}

QString Client::getHost()
{
	return hostname;
}

int Client::getPort()
{
	return port;
}

void Client::sendStateUpdate(int x, int y, teamid_t team)
{
	if (socket->state() != QTcpSocket::ConnectedState)
	{
		return;
	}

	NetworkMessage msg = UpdateMessage{x, y, team};
	stream << msg;
}

void Client::start()
{
	socket->connectToHost(hostname, port, QTcpSocket::ReadWrite, QTcpSocket::AnyIPProtocol);

	bool connectedStatus = socket->waitForConnected();

	if (!connectedStatus)
	{
		return;
	}
	if (socket->state() != QTcpSocket::ConnectedState)
	{
		return;
	}

	emit connected();
}

void Client::stop()
{
	socket->disconnectFromHost();
	emit disconnected();
}

void Client::readyRead()
{
	stream.startTransaction();
	NetworkMessage msg;
	stream >> msg;

	if (!stream.commitTransaction())
		return;

	if (const InitMessage* initMessage = std::get_if<InitMessage>(&msg))
	{
		emit initGame(initMessage->width, initMessage->height, initMessage->tiles, initMessage->teams);
	}
	else if (const UpdateMessage* updateMessage = std::get_if<UpdateMessage>(&msg))
	{
		emit setState(updateMessage->x, updateMessage->y, updateMessage->team);
	}
}
