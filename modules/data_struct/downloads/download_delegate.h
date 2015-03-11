#ifndef DOWNLOAD_DELEGATE_H
#define DOWNLOAD_DELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
//#include <QApplication>
#include <qprogressbar.h>
#include <QStyleOption>

#include "download_model_item.h"

//#include "download_editor.h"

class DownloadDelegate : public QStyledItemDelegate {
    Q_OBJECT

public:
    DownloadDelegate(QWidget * parent = 0) : QStyledItemDelegate(parent) {}

    void paint(QPainter * painter, const QStyleOptionViewItem & option,
               const QModelIndex & index) const;
//    QSize sizeHint(const QStyleOptionViewItem &option,
//                   const QModelIndex &index) const;
//    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
//                          const QModelIndex &index) const;
//    void setEditorData(QWidget *editor, const QModelIndex &index) const;
//    void setModelData(QWidget *editor, QAbstractItemModel *model,
//                      const QModelIndex &index) const;

//private slots:
//    void commitAndCloseEditor();
};

#endif // DOWNLOAD_DELEGATE_H