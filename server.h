#ifndef SERVER_H
#define SERVER_H

#include "game.h"
#include "networkmessages.h"
#include <set>
#include <QThread>
#include <QTcpSocket>
#include <QTcpServer>

class Server : public QObject
{
	Q_OBJECT
public:
	Server(int port, QObject* parent = nullptr);

	void setGame(Game*);
	int getPort();
	void setPort(int);
private:
	QTcpServer* server;
	std::set<QTcpSocket*> clients;
	int port;
	Game* currentGame;

	NetworkMessage getInitMessage();
	void sendMessage(QTcpSocket*, NetworkMessage&);
public slots:
	void broadcastNewGame();
	void broadcastStateUpdate(int, int, teamid_t);
	bool start();
	void stop();

private slots:
	void newConnection();
	void disconnected();
	void readyRead();

signals:
	void stateUpdate(int, int, teamid_t);
};

#endif // SERVER_H
