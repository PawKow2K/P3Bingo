#include "game.h"
#include <QSize>
#include <QRandomGenerator>

Game::Game(QObject *parent)
	: QAbstractTableModel(parent)
{}

QVariant Game::headerData(int section, Qt::Orientation orientation, int role) const
{
	return "";
}

int Game::rowCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;

	return height;
}

int Game::columnCount(const QModelIndex &parent) const
{
	if (parent.isValid())
		return 0;

	return width;
}

QVariant Game::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	auto& tile = field.at(index.row()).at(index.column());

	if (role == Qt::DisplayRole)
	{
		return tile.text;
	}
	if (role == Qt::BackgroundRole)
	{
		if (tile.team == TEAM_UNCLAIMED)
		{
			return unclaimedBrush;
		}
		else
		{
			return teamBrushes.at(tile.team);
		}
	}

	return {};
}

bool Game::setData(const QModelIndex &index, const QVariant &value, int role)
{
	auto& tile = field.at(index.row()).at(index.column());
	if (role == CustomRoles::TryChangeTileRole)
	{
		if (tile.team == myTeam)
		{
			tile.team = TEAM_UNCLAIMED;
			emit tileUpdated(index.row(), index.column(), TEAM_UNCLAIMED);
		}
		else if (tile.team == TEAM_UNCLAIMED)
		{
			tile.team = myTeam;
			emit tileUpdated(index.row(), index.column(), myTeam);
		}
		else
		{
			return false;
		}
	}
	else if (role == CustomRoles::ForceChangeTileRole)
	{
		tile.team = qvariant_cast<teamid_t>(value);
	}
	emit dataChanged(index, index, {role});
	return true;
}

Qt::ItemFlags Game::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;

	return QAbstractItemModel::flags(index);
}

void Game::reset()
{
	auto newList = goalList;
	std::shuffle(newList.begin(), newList.end(), *QRandomGenerator::global());
	if (newList.size() == 0)
	{
		newList.emplace_back("");
	}
	{
		int count = newList.size();
		while(newList.size() < width * height)
		{
			newList.emplace_back(newList.at(QRandomGenerator::global()->bounded(count)));
		}
	}
	field.clear();
	for (int row = 0, id = 0; row < height; row++)
	{
		auto& v = field.emplace_back();
		for (int col = 0; col < height; col++, id++)
		{
			v.emplace_back(newList.at(id), TEAM_UNCLAIMED);
		}
	}
}

void Game::setGoalList(QStringList list)
{
	goalList = list;
}

std::vector<Tile> Game::getCurrentBoard()
{
	std::vector<Tile> res;
	for (int row = 0, id = 0; row < height; row++)
	{
		for (int col = 0; col < height; col++)
		{
			res.push_back(field.at(row).at(col));
		}
	}
	return res;
}

std::vector<TeamData> Game::getTeams()
{
	return teams;
}

void Game::setBoard(int newWidth, int newHeight, std::vector<Tile> tiles)
{
	field.clear();
	setDimensions(newWidth, newHeight);
	for (int row = 0; row < height; row++)
	{
		auto& v = field.emplace_back();
		v.insert(
			v.end(),
			std::make_move_iterator(tiles.begin() + row * width),
			std::make_move_iterator(tiles.begin() + (row + 1) * width)
			);
	}
	emit layoutChanged();
}

void Game::setTile(int x, int y, teamid_t team)
{
	setData(createIndex(x, y), team, CustomRoles::ForceChangeTileRole);
}

void Game::setDimensions(int w, int h)
{
	width = w;
	height = h;
	emit layoutChanged();
}

void Game::setTeams(std::vector<TeamData> newTeams)
{
	teams = newTeams;
	teamBrushes.clear();
	for (auto& team : teams)
	{
		QColor color = team.color;
		color.setAlpha(127);
		teamBrushes.emplace_back(color, Qt::SolidPattern);
	}
}
