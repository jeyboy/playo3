#ifndef DOWNLOAD_DELEGATE_H
#define DOWNLOAD_DELEGATE_H

#include <qstyleditemdelegate.h>
#include <qpainter>
#include <qprogressbar.h>
#include <qstyleoption.h>
#include <qstringbuilder.h>

#include "download_model_item.h"

//#include "download_editor.h"

class DownloadDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    inline DownloadDelegate(QWidget * parent = 0) : QStyledItemDelegate(parent) {}

    void paint(QPainter * painter, const QStyleOptionViewItem & option,
               const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const;

//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
//                          const QModelIndex &index) const;
//    void setEditorData(QWidget *editor, const QModelIndex &index) const;
//    void setModelData(QWidget *editor, QAbstractItemModel *model,
//                      const QModelIndex &index) const;

//private slots:
//    void commitAndCloseEditor();
};

#endif // DOWNLOAD_DELEGATE_H
