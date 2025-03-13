#include "mainwindow.h"

#include <QApplication>
#include <QSettings>

int main(int argc, char *argv[])
{
	QApplication::setApplicationName("P3Bingo");
	QApplication::setOrganizationName("PawKow2K");
	QSettings settings(QSettings::IniFormat, QSettings::UserScope, QApplication::organizationName(), QApplication::applicationName());
	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
