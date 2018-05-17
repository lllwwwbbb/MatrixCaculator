#ifndef REALEDITDELEGATE_H
#define REALEDITDELEGATE_H

#include <QItemDelegate>
#include <QObject>
#include "def.h"


class RealEditDelegate : public QItemDelegate
{
    Q_OBJECT

public:
    RealEditDelegate(QObject *parent = NULL);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
    void updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                              const QModelIndex &index) const;
};

#endif // REALEDITDELEGATE_H
