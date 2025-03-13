#include "teamdata.h"
#include <QDataStream>

QDataStream& operator>> (QDataStream& stream, TeamData& team)
{
	stream >> team.name >> team.color;
	return stream;
}

QDataStream& operator<< (QDataStream& stream, TeamData team)
{
	stream << team.name << team.color;
	return stream;
}
