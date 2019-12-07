#ifndef GAMEITEMDELEGATE_H
#define GAMEITEMDELEGATE_H

#include <QStyledItemDelegate>

class GameItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit GameItemDelegate(QObject *parent = nullptr);

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QString displayText(const QVariant &value, const QLocale &locale) const;
};

#endif // GAMEITEMDELEGATE_H
