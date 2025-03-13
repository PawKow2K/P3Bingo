#include "mainwindow.h"
#include "editlist.h"
#include "tiledelegate.h"
#include "ui_mainwindow.h"
#include "connectionipinput.h"
#include "hostportinput.h"
#include <QStandardPaths>
#include <QDir>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	client = new Client("::1", 2137);
	server = new Server(2137);

	currentGame.setTeams(teamData());
	server->setGame(&currentGame);

	auto board = ui->bingoBoard;

	board->verticalHeader()->setStretchLastSection(true);
	board->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	board->horizontalHeader()->setStretchLastSection(true);
	board->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

	board->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	board->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	board->setAutoScroll(false);

	board->setSelectionMode(QAbstractItemView::NoSelection);

	board->setItemDelegate(new TileDelegate(this));

	loadLists();
	loadSettings();

	connect(client, &Client::initGame, this, &MainWindow::initGame);
	connect(client, &Client::setState, this, &MainWindow::stateUpdate);
	connect(client, &Client::connected, this, &MainWindow::connectedToHost);
	connect(client, &Client::disconnected, this, &MainWindow::disconnectedFromHost);

	connect(server, &Server::stateUpdate, this, &MainWindow::stateUpdate);

	connect(&currentGame, &Game::tileUpdated, client, &Client::sendStateUpdate);
	connect(&currentGame, &Game::tileUpdated, server, &Server::broadcastStateUpdate);
}

void MainWindow::resetBoard()
{
	currentGame.setDimensions(5, 5);
	currentGame.setGoalList(lists[currentList()]);
	currentGame.reset();
	ui->bingoBoard->setModel(&currentGame);
	ui->bingoBoard->update();

	server->setGame(&currentGame);
	server->broadcastNewGame();
}

void MainWindow::launchEditor()
{
	EditList dialog(currentList(), lists[currentList()]);
	dialog.setModal(true);
	if (dialog.exec() == QDialog::Accepted)
	{
		QStringList list = dialog.getList();
		QString listName = dialog.getListName();

		lists.erase(currentList());
		listsToRemove.insert(currentList());
		lists[listName] = list;
		ui->listSelector->setItemText(ui->listSelector->currentIndex(), listName);
		listsToRemove.erase(listName);
	}
}

void MainWindow::launchNewEditor()
{
	EditList dialog("", {});
	dialog.setModal(true);
	if (dialog.exec() == QDialog::Accepted)
	{
		QStringList list = dialog.getList();
		QString listName = dialog.getListName();

		lists[listName] = list;
		ui->listSelector->addItem(listName);
		ui->listSelector->setCurrentIndex(ui->listSelector->count() - 1);
		listsToRemove.erase(listName);
	}
}

QString MainWindow::currentList()
{
	return ui->listSelector->currentText();
}

void MainWindow::loadLists()
{
	QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	QDir appDataDir(appDataPath);
	if (!appDataDir.exists())
	{
		appDataDir.mkpath(appDataPath);
	}

	if (!appDataDir.exists("lists"))
	{
		appDataDir.mkdir("lists");
	}
	appDataDir.cd("lists");

	for (auto& file : QDirListing(appDataDir.absolutePath(), QDirListing::IteratorFlag::FilesOnly))
	{
		QStringList list;
		QFile listToLoad(file.absoluteFilePath());
		if (!listToLoad.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			continue;
		}

		QTextStream in(&listToLoad);
		while(!in.atEnd())
		{
			list.append(in.readLine());
		}

		lists[file.fileName()] = list;
		ui->listSelector->addItem(file.fileName());
	}
}

void MainWindow::loadSettings()
{
	QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	QDir appDataDir(appDataPath);
	if (!appDataDir.exists())
	{
		return;
	}

	QFile settingsFile(appDataDir.absoluteFilePath("settings.json"));
	if (settingsFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QJsonObject settings = QJsonDocument::fromJson(QString(settingsFile.readAll()).toUtf8()).object();
		QJsonValue clientIp = settings.value("last_client_ip");
		QJsonValue clientPort = settings.value("last_client_port");
		QJsonValue serverPort = settings.value("last_server_port");

		if (!clientIp.isUndefined() && !clientPort.isUndefined())
		{
			client->setHost(clientIp.toString(), clientPort.toInt());
		}

		if (!serverPort.isUndefined())
		{
			server->setPort(serverPort.toInt());
		}

		settingsFile.close();
	}
}

void MainWindow::saveLists()
{
	QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	QDir appDataDir(appDataPath);
	if (!appDataDir.exists())
	{
		appDataDir.mkpath(appDataPath);
	}

	if (!appDataDir.exists("lists"))
	{
		appDataDir.mkdir("lists");
	}
	appDataDir.cd("lists");

	for (auto& [listName, list] : lists)
	{
		if (listName == "")
		{
			continue;
		}

		QFile listToSave(appDataDir.absoluteFilePath(listName));
		listToSave.remove();
		if (!listToSave.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			continue;
		}

		QTextStream out(&listToSave);
		for (auto& line : list)
		{
			out << line << '\n';
		}
	}

	for (auto& filename : listsToRemove)
	{
		QFile listToRemove(appDataDir.absoluteFilePath(filename));
		listToRemove.remove();
	}
}

void MainWindow::saveSettings()
{
	QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
	QDir appDataDir(appDataPath);
	if (!appDataDir.exists())
	{
		appDataDir.mkpath(appDataPath);
	}

	QFile settingsFile(appDataDir.absoluteFilePath("settings.json"));
	if (!settingsFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		return;
	}

	QJsonObject settings;
	settings["last_client_ip"] = client->getHost();
	settings["last_client_port"] = client->getPort();
	settings["last_server_port"] = server->getPort();

	QJsonDocument settingsDoc(settings);

	settingsFile.write(settingsDoc.toJson());

	settingsFile.close();
}

std::vector<TeamData> MainWindow::teamData()
{
	return {
		{"Default team", Qt::yellow}
	};
}

void MainWindow::closeEvent(QCloseEvent* event)
{
	saveSettings();
	saveLists();

	server->stop();
	client->stop();

	QWidget::closeEvent(event);
}

void MainWindow::removeCurrentList()
{
	QString currentListName = currentList();
	if (currentListName == "")
	{
		return;
	}

	lists.erase(currentListName);
	listsToRemove.insert(currentListName);
	ui->listSelector->removeItem(ui->listSelector->findText(currentListName));
}

void MainWindow::connectionToggle()
{
	if (networkStatus == NetworkStatus::Connected)
	{
		client->stop();
	}
	else if (networkStatus == NetworkStatus::Disconnected)
	{
		ConnectionIPInput dialog(client->getHost(), client->getPort());
		dialog.setModal(true);
		if (dialog.exec() == QDialog::Accepted)
		{
			bool portParsed;
			QString hostname = dialog.getIp();
			int port = dialog.getPort().toInt(&portParsed);

			if (portParsed)
			{
				client->setHost(hostname, port);

				client->start();
			}
		}
	}
}

void MainWindow::hostingToggle()
{
	if (networkStatus == NetworkStatus::Disconnected)
	{
		HostPortInput dialog(server->getPort());
		dialog.setModal(true);
		if (dialog.exec() == QDialog::Accepted)
		{
			bool portParsed;
			int port = dialog.getPort().toInt(&portParsed);

			if (portParsed)
			{
				server->setPort(port);
				bool success = server->start();
				if (success)
				{
					ui->connectButton->setVisible(false);
					networkStatus = NetworkStatus::Hosting;

					ui->hostButton->setText("Stop hosting");
				}
			}
		}
	}
	else if (networkStatus == NetworkStatus::Hosting)
	{
		server->stop();
		ui->connectButton->setVisible(true);
		networkStatus = NetworkStatus::Disconnected;

		ui->hostButton->setText("Start hosting");
	}
}

void MainWindow::initGame(int width, int height, std::vector<Tile> tiles, std::vector<TeamData> teams)
{
	currentGame.setTeams(teams);
	currentGame.setBoard(width, height, tiles);
	ui->bingoBoard->setModel(&currentGame);
	ui->bingoBoard->update();
}

void MainWindow::stateUpdate(int x, int y, teamid_t team)
{
	currentGame.setTile(x, y, team);
}

void MainWindow::connectedToHost()
{
	ui->hostButton->setVisible(false);
	ui->hostSidebar->setVisible(false);

	networkStatus = NetworkStatus::Connected;

	ui->connectButton->setText("Disconnect");
}

void MainWindow::disconnectedFromHost()
{
	ui->hostButton->setVisible(true);
	ui->hostSidebar->setVisible(true);

	networkStatus = NetworkStatus::Disconnected;

	ui->connectButton->setText("Connect");
}

MainWindow::~MainWindow()
{
	delete ui;
}
