#ifndef RELATIONS_DELEGATE_H
#define RELATIONS_DELEGATE_H

#include <qstyleditemdelegate.h>
#include <qpainter.h>
#include <qprogressbar.h>
#include <qstyleoption.h>
#include <qstringbuilder.h>

#include "modules/core/web/utils/web_manager.h"

class RelationsDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    inline RelationsDelegate(QWidget * parent = 0) : QStyledItemDelegate(parent) {}

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;
protected slots:

};

#endif // RELATIONS_DELEGATE_H
