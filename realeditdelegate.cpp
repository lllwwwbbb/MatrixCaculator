#include "realeditdelegate.h"
#include <QLineEdit>

RealEditDelegate::RealEditDelegate(QObject *parent)
    :QItemDelegate(parent)
{

}

QWidget *RealEditDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    QLineEdit *pLineEdit = new QLineEdit(parent);
    QRegExp regExp(REGEXP_REAL);
    pLineEdit->setValidator(new QRegExpValidator(regExp, parent));
    return pLineEdit;
}

void RealEditDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString data =  index.model()->data(index, Qt::EditRole).toString();
    QLineEdit *pEdit = (QLineEdit *)editor;
    pEdit->setText(data);
}

void RealEditDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QLineEdit *pEdit = (QLineEdit *)editor;
    QString data = pEdit->text();
    model->setData(index, data, Qt::EditRole);
    model->setData(index, data, Qt::DisplayRole);
}

void RealEditDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}
