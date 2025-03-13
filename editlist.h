#ifndef EDITLIST_H
#define EDITLIST_H

#include <QDialog>

namespace Ui {
class EditList;
}

class EditList : public QDialog
{
	Q_OBJECT

public:
	explicit EditList(QString listName, QStringList, QWidget *parent = nullptr);
	~EditList();

	QStringList getList();
	QString getListName();
private:
	Ui::EditList *ui;
};

#endif // EDITLIST_H
