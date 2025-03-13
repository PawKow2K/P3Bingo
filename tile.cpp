#include "tile.h"
#include <QDataStream>

QDataStream& operator>> (QDataStream& stream, Tile& tile)
{
	stream >> tile.text >> tile.team;
	return stream;
}

QDataStream& operator<< (QDataStream& stream, Tile tile)
{
	stream << tile.text << tile.team;
	return stream;
}
