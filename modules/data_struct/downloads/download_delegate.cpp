#include "download_delegate.h"

void DownloadDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    int progressPercentage = index.data();

    QStyleOptionProgressBarV2 progressBarOption;
    progressBarOption.rect = QRect(option.rect.x() + 10, option.rect.y() + 2 , option.rect.width() - 16, option.rect.height() - 4);
    progressBarOption.text = index.model() -> data(index, Qt::DisplayRole).toString();
    progressBarOption.textAlignment = Qt::AlignLeft | Qt::AlignHCenter | Qt::AlignJustify;
    progressBarOption.textVisible = true;
    progressBarOption.minimum = 0;
    progressBarOption.maximum = 100;
    progressBarOption.progress = progressPercentage;
    QPalette pal = progressBarOption.palette;
    pal.setColor(QPalette::HighlightedText, QColor::fromRgb(0, 0, 0));
    progressBarOption.palette = pal;

//    if(option.state & QStyle::State_Selected) {

//    }

    QApplication::style() -> drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
}

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
