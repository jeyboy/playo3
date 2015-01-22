#ifndef MODEL_ITEM_DELEGATE_H
#define MODEL_ITEM_DELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOption>
#include <QTreeView>
#include <QCheckBox>

#include "model_item_parts/item_fields.h"

class ModelItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit ModelItemDelegate(QObject *parent = 0);

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

//    QWidget *createEditor(QWidget *parent,
//                          const QStyleOptionViewItem &option,
//                          const QModelIndex &index) const;

//    void setEditorData(QWidget *editor, const QModelIndex &index) const;
//    void setModelData(QWidget *editor,
//                      QAbstractItemModel *model,
//                      const QModelIndex &index) const;

//    void updateEditorGeometry(QWidget *editor,
//                              const QStyleOptionViewItem &option,
//                              const QModelIndex &index) const;

    void usuall(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index) const;

    void progress(QPainter* painter, const QStyleOptionViewItem& option,
               const QModelIndex& index, int progressPercentage) const;

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const;

private:
    void drawCheckbox(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    QCheckBox templateCheckbox;
    QColor hoverColor;

//    static QPainterPath backroundRectPath(const QRect &rect, int offset);
//    static QPainterPath roundRectPath(const QRect &rect, int offset);
};

#endif // MODEL_ITEM_DELEGATE_H
