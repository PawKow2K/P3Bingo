#include "tiledelegate.h"
#include "bingodef.h"
#include <QLabel>
#include <QPainter>
#include <QEvent>
#include <QSinglePointEvent>
#include <QRgb>
#include <QFontMetrics>

TileDelegate::TileDelegate(QObject* parent) : QStyledItemDelegate(parent), font("Helvetica")
{
}

void TileDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QBrush brush = index.data(Qt::BackgroundRole).value<QBrush>();
	painter->fillRect(option.rect, brush);

	QFont localFont = font;

	auto text = index.data().toString();

	int textFontSize = 2;
	localFont.setPointSize(textFontSize);

	QFontMetrics fontMetrics(localFont);
	QRect rect = fontMetrics.boundingRect(option.rect, Qt::AlignHCenter | Qt::TextWordWrap, text);
	while (option.rect.contains(rect))
	{
		textFontSize += 1;
		localFont.setPointSize(textFontSize);
		fontMetrics = QFontMetrics(localFont);
		rect = fontMetrics.boundingRect(option.rect, Qt::AlignHCenter | Qt::TextWordWrap, text);
	}

	textFontSize -= 1;
	localFont.setPointSize(textFontSize);

	painter->setFont(localFont);
	painter->drawText(option.rect, Qt::AlignCenter | Qt::TextWordWrap, text);
}

bool TileDelegate::editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index)
{
	if (event->isSinglePointEvent())
	{
		QSinglePointEvent* e = dynamic_cast<QSinglePointEvent*>(event);
		auto button = e->button();

		if (button == Qt::LeftButton && e->isBeginEvent())
		{
			return model->setData(index, 0, CustomRoles::TryChangeTileRole);
		}
	}
	return false;
}
