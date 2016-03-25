#ifndef MODEL_ITEM_DELEGATE_H
#define MODEL_ITEM_DELEGATE_H

#include <qstyleditemdelegate.h>
#include <qpainter.h>
#include <qstyleoption.h>
#include <qtreeview.h>
#include <qcheckbox.h>

#include "modules/core/core_parts/part_mixes/item_fields.h"
#include "modules/core/data_sub_types.h"

#define SELECTION_ITER 50
#define PIXMAP(path, size) \
    QPixmap(path).scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation)

class ModelItemDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit ModelItemDelegate(QObject * parent = 0);
    inline ~ModelItemDelegate() {
        delete fmf;
        delete fmfInfo;
    }

//    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;

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

    inline void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
        if (Settings::obj().itemPresentType() == 1)
            paintVar1(painter, option, index);
        else
            paintVar2(painter, option, index);
    }

protected:
    void paintVar1(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void paintVar2(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;

public slots:
    void recalcAttrs(int item_icon_size);

private:
    void drawCheckbox(bool is_container, QVariant & checkable, QPainter * painter, const QStyleOptionViewItem & option) const;
    QCheckBox templateCheckbox;
    QColor hoverColor;

    QFont itemFont, itemInfoFont;
    QFontMetrics * fmf, * fmfInfo;
    QHash<int, QPixmap> icons;
    int icon_size, ico_mini, extra_font_size, state_width;
    QString extra_font_name, titleTemplate;


//    static QPainterPath backroundRectPath(const QRect & rect, int offset);
//    static QPainterPath roundRectPath(const QRect & rect, int offset);
};

#endif // MODEL_ITEM_DELEGATE_H
