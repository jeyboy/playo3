#ifndef HOTKEY_DELEGATE_H
#define HOTKEY_DELEGATE_H

#include <qstyleditemdelegate.h>

#include "hotkey_editor.h"

class HotkeyDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    HotkeyDelegate(QWidget * parent = 0) : QStyledItemDelegate(parent) {}

    void paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    QSize sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const;
    QWidget * createEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    void setEditorData(QWidget * editor, const QModelIndex & index) const;
    void setModelData(QWidget * editor, QAbstractItemModel * model, const QModelIndex & index) const;
private slots:
    void commitAndCloseEditor();
};

#endif // HOTKEY_DELEGATE_H
