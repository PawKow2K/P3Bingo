#ifndef CLIENT_H
#define CLIENT_H

#include "teamdata.h"
#include "tile.h"
#include <QTcpSocket>
#include <QDataStream>

class Client : public QObject
{
	Q_OBJECT
public:
	Client(QString ip, int port, QObject* parent = nullptr);

	void setHost(QString hostname, int port);
	QString getHost();
	int getPort();
private:
	QTcpSocket* socket;
	QDataStream stream;

	QString hostname;
	int port;

signals:
	void setState(int, int, teamid_t);
	void initGame(int, int, std::vector<Tile>, std::vector<TeamData>);
	void connected();
	void disconnected();

public slots:
	void sendStateUpdate(int, int, teamid_t);
	void start();
	void stop();

private slots:
	void readyRead();
};

#endif // CLIENT_H
