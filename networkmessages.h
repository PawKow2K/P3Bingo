#ifndef NETWORKMESSAGES_H
#define NETWORKMESSAGES_H

#include "teamdata.h"
#include "tile.h"
#include <QStringList>
#include <QDataStream>

struct InitMessage
{
	qint32 width;
	qint32 height;
	std::vector<Tile> tiles;
	std::vector<TeamData> teams;
};

QDataStream& operator>> (QDataStream&, InitMessage&);
QDataStream& operator<< (QDataStream&, InitMessage);

struct UpdateMessage
{
	qint32 x;
	qint32 y;
	qint32 team;
};

QDataStream& operator>> (QDataStream&, UpdateMessage&);
QDataStream& operator<< (QDataStream&, UpdateMessage);


using NetworkMessage = std::variant<InitMessage, UpdateMessage>;

QDataStream& operator>> (QDataStream&, NetworkMessage&);
QDataStream& operator<< (QDataStream&, NetworkMessage);

#endif // NETWORKMESSAGES_H
