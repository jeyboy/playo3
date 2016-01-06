#ifndef DOWNLOAD_DELEGATE_H
#define DOWNLOAD_DELEGATE_H

#include <qstyleditemdelegate.h>
#include <qpainter.h>
#include <qprogressbar.h>
#include <qstyleoption.h>
#include <qstringbuilder.h>

class DownloadDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    inline DownloadDelegate(QWidget * parent = 0) : QStyledItemDelegate(parent) {}

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;
};

#endif // DOWNLOAD_DELEGATE_H
