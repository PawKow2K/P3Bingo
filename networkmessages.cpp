#include "networkmessages.h"
#include <qdebug.h>

namespace {

template<typename T>
struct closure
{
	auto operator()()
	{
		return [](QDataStream& stream) {
			T msg;
			stream >> msg;
			return msg;
		};
	}
};

template<typename... Ts>
static const std::array<std::function<std::variant<Ts...>(QDataStream&)>, sizeof...(Ts)> readers_impl {
	closure<Ts>()() ...
};

template<typename T>
struct ReaderFactory{
	std::array<T, 0> operator()()
	{
		return {};
	}
};

template<typename... Ts>
struct ReaderFactory<std::variant<Ts...>> {
	std::array<std::function<std::variant<Ts...>(QDataStream&)>, sizeof...(Ts)> operator()()
	{
		return {closure<Ts>()() ...};
	}
};
}


template<typename T>
auto readers = ReaderFactory<T>{}();

QDataStream& operator>> (QDataStream& stream, InitMessage& msg)
{
	stream >> msg.width;
	stream >> msg.height;
	int tile_count = msg.width * msg.height;
	msg.tiles.resize(tile_count);
	for (int i = 0; i < tile_count; i++)
	{
		stream >> msg.tiles[i];
	}
	qsizetype team_count;
	stream >> team_count;
	msg.teams.resize(team_count);
	for (int i = 0; i < team_count; i++)
	{
		stream >> msg.teams[i];
	}
	return stream;
}

QDataStream& operator<<(QDataStream& stream, InitMessage msg)
{
	stream << msg.width;
	stream << msg.height;
	for (int i = 0; i < msg.width * msg.height; i++)
	{
		stream << msg.tiles[i];
	}
	stream << static_cast<qsizetype>(msg.teams.size());
	for (int i = 0; i < msg.teams.size(); i++)
	{
		stream << msg.teams[i];
	}
	return stream;
}

QDataStream& operator>>(QDataStream& stream, UpdateMessage& msg)
{
	stream >> msg.x >> msg.y >> msg.team;
	return stream;
}

QDataStream& operator<<(QDataStream& stream, UpdateMessage msg)
{
	stream << msg.x << msg.y << msg.team;
	return stream;
}

QDataStream& operator>>(QDataStream& stream, NetworkMessage& msg)
{
	qsizetype index;
	stream >> index;
	msg = readers<NetworkMessage>[index](stream);
	return stream;
}

QDataStream& operator<<(QDataStream& stream, NetworkMessage msg)
{
	stream << static_cast<qsizetype>(msg.index());
	std::visit([&](auto&& arg){ stream << arg;}, msg);
	return stream;
}
