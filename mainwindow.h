#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "game.h"
#include <QMainWindow>
#include <set>
#include "client.h"
#include "server.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private:
	Ui::MainWindow *ui;
	Game currentGame;

	std::map<QString, QStringList> lists;
	QString currentList();
	std::set<QString> listsToRemove;

	void loadLists();
	void loadSettings();

	void closeEvent(QCloseEvent* event) override;

	void saveLists();
	void saveSettings();

	Client* client;
	Server* server;

	NetworkStatus networkStatus = NetworkStatus::Disconnected;

	std::vector<TeamData> teamData();

public slots:
	void resetBoard();
	void launchEditor();
	void launchNewEditor();
	void removeCurrentList();
	void connectionToggle();
	void hostingToggle();
	void initGame(int, int, std::vector<Tile>, std::vector<TeamData>);
	void stateUpdate(int, int, teamid_t);
	void connectedToHost();
	void disconnectedFromHost();
};
#endif // MAINWINDOW_H
