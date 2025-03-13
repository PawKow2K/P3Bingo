#ifndef BINGODEF_H
#define BINGODEF_H

#include <qnamespace.h>
#include <concepts>

enum CustomRoles {
	TryChangeTileRole = Qt::UserRole,
	ForceChangeTileRole,
	TeamColorRole,
};

enum NetworkStatus {
	Disconnected,
	Connected,
	Hosting,
};

// teams will be stored from 0 to MAX-1
using teamid_t = int;

static_assert(std::integral<teamid_t>, "Team id has to be an integral type");

constexpr teamid_t TEAM_UNCLAIMED = std::numeric_limits<teamid_t>::max();

#endif // BINGODEF_H
