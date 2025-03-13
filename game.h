#ifndef GAME_H
#define GAME_H

#include "teamdata.h"
#include "tile.h"
#include <QAbstractTableModel>
#include <QBrush>

class Game : public QAbstractTableModel
{
	Q_OBJECT

public:
	explicit Game(QObject *parent = nullptr);

	// Header:
	QVariant headerData(int section,
						Qt::Orientation orientation,
						int role = Qt::DisplayRole) const override;

	// Basic functionality:
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	// Editable:
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

	Qt::ItemFlags flags(const QModelIndex &index) const override;

	void reset();

	void setGoalList(QStringList);

	std::vector<Tile> getCurrentBoard();

	std::vector<TeamData> getTeams();

	void setBoard(int, int, std::vector<Tile>);

	void setTeams(std::vector<TeamData>);

	void setTile(int, int, teamid_t);
	void setDimensions(int, int);
signals:
	void tileUpdated(int, int, teamid_t);

private:
	int width = 0;
	int height = 0;

	std::vector<TeamData> teams;
	std::vector<QBrush> teamBrushes;
	QBrush unclaimedBrush{};
	teamid_t myTeam = 0;

	std::vector<std::vector<Tile>> field;

	QStringList goalList;
};

#endif // GAME_H
