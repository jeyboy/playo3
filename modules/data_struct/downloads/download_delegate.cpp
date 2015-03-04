//#include "download_delegate.h"

//void HotkeyDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
//    QStyledItemDelegate::paint(painter, option, index);
//}

//QSize HotkeyDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
//    return QStyledItemDelegate::sizeHint(option, index);
//}

//QWidget * HotkeyDelegate::createEditor(QWidget *parent,
//                                     const QStyleOptionViewItem &,
//                                     const QModelIndex & index) const {

//    HotkeyEditor * editor = new HotkeyEditor(index.data(Qt::UserRole).toInt(), parent);
//    connect(editor, SIGNAL(editingFinished()), this, SLOT(commitAndCloseEditor()));

//    return editor;
//}

//void HotkeyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
//    ((HotkeyEditor *)editor) -> setKeySequence(QKeySequence(index.data().toString()));
////    QStyledItemDelegate::setEditorData(editor, index);
//}

//void HotkeyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
//    QStyledItemDelegate::setModelData(editor, model, index);
//}

//void HotkeyDelegate::commitAndCloseEditor() {
////    StarEditor *editor = qobject_cast<StarEditor *>(sender());
//    QWidget * editor = qobject_cast<QWidget *>(sender());
//    emit commitData(editor);
//    emit closeEditor(editor);
//}
