#include "gameitemdelegate.h"

#include <QPainter>

#include "define.h"

GameItemDelegate::GameItemDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

QSize GameItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    return QSize(0, 48);
}

void GameItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QFont font = painter->font();
    int y = option.rect.y();
    int width = painter->device()->width();
    int height = option.rect.height();

    QString title = index.data(GameTitleRole).toString();
    QIcon icon = index.data(GameIconRole).value<QIcon>();

    painter->save();
    QColor selectedColor;
    if (option.state & QStyle::State_Selected)
        selectedColor.setNamedColor("#efefef");
    else
        selectedColor.setNamedColor("#fff");

    painter->setPen(selectedColor);
    painter->setBrush(selectedColor);
    painter->drawRect(0, y, width, height);
    painter->restore();

    painter->save();
    font.setPixelSize(14);
    painter->setFont(font);
    painter->drawText(48, y + 8, width - 48, 28, Qt::AlignVCenter | Qt::AlignLeft | Qt::TextWordWrap, title);
    painter->drawPixmap(QRect(8, y + 8, 32, 32), icon.pixmap(32, 32));
    painter->restore();
}

QString GameItemDelegate::displayText(const QVariant &value, const QLocale &locale) const
{
    Q_UNUSED(value)
    Q_UNUSED(locale)

    return QString();
}
