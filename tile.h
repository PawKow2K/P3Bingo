#ifndef TILE_H
#define TILE_H

#include <QString>
#include "bingodef.h"

struct Tile
{
	QString text = "";
	teamid_t team = TEAM_UNCLAIMED;
};

QDataStream& operator>> (QDataStream&, Tile&);
QDataStream& operator<< (QDataStream&, Tile);

#endif // TILE_H
