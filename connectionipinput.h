#ifndef CONNECTIONIPINPUT_H
#define CONNECTIONIPINPUT_H

#include <QDialog>

namespace Ui {
class ConnectionIPInput;
}

class ConnectionIPInput : public QDialog
{
	Q_OBJECT

public:
	explicit ConnectionIPInput(QString ip, int port, QWidget *parent = nullptr);
	~ConnectionIPInput();

	QString getIp();
	QString getPort();

private:
	Ui::ConnectionIPInput *ui;
};

#endif // CONNECTIONIPINPUT_H
