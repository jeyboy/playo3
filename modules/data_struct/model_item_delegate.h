#ifndef MODEL_ITEM_DELEGATE_H
#define MODEL_ITEM_DELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QStyleOption>
#include <QTreeView>
#include <QCheckBox>

#include "model_item_parts/item_fields.h"
#include "model_item_parts/item_types.h"

#define SELECTION_ITER 9

class ModelItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit ModelItemDelegate(QObject * parent = 0);
    inline ~ModelItemDelegate() {
        delete fmf;
        delete fmfInfo;
    }

    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

//    QWidget *createEditor(QWidget *parent,
//                          const QStyleOptionViewItem &option,
//                          const QModelIndex &index) const;

//    void setEditorData(QWidget *editor, const QModelIndex &index) const;
//    void setModelData(QWidget *editor,
//                      QAbstractItemModel * model,
//                      const QModelIndex &index) const;

//    void updateEditorGeometry(QWidget * editor,
//                              const QStyleOptionViewItem & option,
//                              const QModelIndex & index) const;

   void paint(QPainter * painter, const QStyleOptionViewItem & option,
               const QModelIndex & index) const;

public slots:
    void recalcAttrs(int item_icon_size);

private:
    void drawCheckbox(bool is_container, QVariant & checkable, QPainter * painter, const QStyleOptionViewItem & option) const;
    QCheckBox templateCheckbox;
    QColor hoverColor;

    QFont itemFont, itemInfoFont;
    QFontMetrics * fmf, * fmfInfo;
    QHash<int, QPixmap> icons;
    int icon_size, ico_mini;

//    static QPainterPath backroundRectPath(const QRect & rect, int offset);
//    static QPainterPath roundRectPath(const QRect & rect, int offset);
};

#endif // MODEL_ITEM_DELEGATE_H
