#ifndef TILEDELEGATE_H
#define TILEDELEGATE_H

#include <QFont>
#include <QStyledItemDelegate>

class TileDelegate : public QStyledItemDelegate
{
	Q_OBJECT
public:
	TileDelegate(QObject* parent = nullptr);

	void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;

private:
	bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

	QFont font;
};

#endif // TILEDELEGATE_H
