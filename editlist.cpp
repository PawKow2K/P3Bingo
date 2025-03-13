#include "editlist.h"
#include "ui_editlist.h"
#include <QUuid>

EditList::EditList(QString listName, QStringList list, QWidget *parent)
	: QDialog(parent)
	, ui(new Ui::EditList)
{
	ui->setupUi(this);
	ui->listInput->setPlainText(list.join('\n'));
	ui->listName->setText(listName);
}

EditList::~EditList()
{
	delete ui;
}

QStringList EditList::getList()
{
	QStringList list = ui->listInput->toPlainText().split('\n', Qt::SkipEmptyParts);
	return list;
}

QString EditList::getListName()
{
	QString listName = ui->listName->text();
	if (listName == "")
	{
		listName = QUuid::createUuid().toString(QUuid::WithoutBraces);
	}
	return listName;
}
