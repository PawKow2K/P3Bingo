#include "connectionipinput.h"
#include "ui_connectionipinput.h"

ConnectionIPInput::ConnectionIPInput(QString ip, int port, QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::ConnectionIPInput)
{
	ui->setupUi(this);
	ui->ipInput->setText(ip);
	ui->portInput->setText(QString::number(port));
}

ConnectionIPInput::~ConnectionIPInput()
{
	delete ui;
}

QString ConnectionIPInput::getIp()
{
	return ui->ipInput->text();
}

QString ConnectionIPInput::getPort()
{
	return ui->portInput->text();
}
