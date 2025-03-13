#ifndef TEAMDATA_H
#define TEAMDATA_H

#include <QString>
#include <QColor>

struct TeamData
{
	QString name;
	QColor color;
};

QDataStream& operator>> (QDataStream&, TeamData&);
QDataStream& operator<< (QDataStream&, TeamData);

#endif // TEAMDATA_H
