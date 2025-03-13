#include "hostportinput.h"
#include "ui_hostportinput.h"

HostPortInput::HostPortInput(int port, QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::HostPortInput)
{
	ui->setupUi(this);
	ui->portInput->setText(QString::number(port));
}

HostPortInput::~HostPortInput()
{
	delete ui;
}

QString HostPortInput::getPort()
{
	return ui->portInput->text();
}
