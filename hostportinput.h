#ifndef HOSTPORTINPUT_H
#define HOSTPORTINPUT_H

#include <QDialog>

namespace Ui {
class HostPortInput;
}

class HostPortInput : public QDialog
{
	Q_OBJECT

public:
	explicit HostPortInput(int port, QWidget *parent = nullptr);
	~HostPortInput();

	QString getPort();

private:
	Ui::HostPortInput *ui;
};

#endif // HOSTPORTINPUT_H
