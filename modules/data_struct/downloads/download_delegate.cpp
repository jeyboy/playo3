#include "download_delegate.h"

QSize DownloadDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const {
    int progressPercentage = index.data(DOWNLOAD_PROGRESS).toInt();

    QSize size = QStyledItemDelegate::sizeHint(option, index);

    if (progressPercentage == -2) {
        size.setHeight(size.height() * 2);
        return size;
    }
    else return size;
}

void DownloadDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    int progressPercentage = index.data(DOWNLOAD_PROGRESS).toInt();

    if (progressPercentage >= 0) {
        QProgressBar renderer;

        renderer.setAttribute(Qt::WA_OpaquePaintEvent, true);
        renderer.setAttribute(Qt::WA_NoSystemBackground, true);
        renderer.setAttribute(Qt::WA_TranslucentBackground, true);

        QString style;
        QSize barSize = option.rect.size() - QSize(6, 4);

        if(option.state & QStyle::State_Selected) {
            style = "QProgressBar { border: 2px solid grey; border-radius: 4px; text-align: center; background-color: #999999; }";
            style += "QProgressBar::chunk { background-color: #87CEFA; /*width: " + QString::number(barSize.width() - 6) + "px;  margin: 0.5px; border-radius: 6px;*/ }";
        } else {
            style = "QProgressBar { border: 2px solid grey; border-radius: 4px; text-align: center; }";
            style += "QProgressBar::chunk { background-color: #05B8CC; /*width: " + QString::number(barSize.width() - 6) + "px;  margin: 0.5px; border-radius: 6px;*/ }";
        }

        renderer.setTextVisible(true);
        renderer.setFormat(option.fontMetrics.elidedText(index.model() -> data(index, Qt::DisplayRole).toString(), Qt::ElideRight, option.rect.width() - 8));

        renderer.resize(barSize);
        renderer.setMinimum(0);
        renderer.setMaximum(100);
        renderer.setValue(progressPercentage);

        renderer.setStyleSheet(style);
        painter -> save();
        painter -> translate(option.rect.topLeft() + QPoint(3, 2));
        renderer.render(painter);
        painter -> restore();



//        QStyleOptionProgressBarV2 progressBarOption;
//        progressBarOption.rect = QRect(option.rect.x() + 3, option.rect.y() + 2 , option.rect.width() - 6, option.rect.height() - 4);
//        progressBarOption.text = progressBarOption.fontMetrics.elidedText(index.model() -> data(index, Qt::DisplayRole).toString(), Qt::ElideRight, progressBarOption.rect.width() - 8);
//        progressBarOption.textAlignment = Qt::AlignCenter;
//        progressBarOption.textVisible = true;
//        progressBarOption.minimum = 0;
//        progressBarOption.maximum = 100;
//        progressBarOption.progress = progressPercentage;
//        progressBarOption.state = option.state;

//        QPalette pal = progressBarOption.palette;
//        pal.setColor(QPalette::HighlightedText, QColor::fromRgb(0, 0, 0));
//        progressBarOption.palette = pal;

////        if(option.state & QStyle::State_Selected) {

////        }

//        QApplication::style() -> drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    } else if (progressPercentage == -2) {
        QRect rect = option.rect;
        rect.setTopLeft(rect.topLeft() + QPoint(4, 1));
        rect.setBottomRight(rect.bottomRight() - QPoint(8, 2));

        QRect textRect(rect);
        textRect.setLeft(rect.left() + 4);
        textRect.setRight(rect.right() - 8);

        int height = textRect.height() / 2;

        textRect.setBottom(textRect.bottom() - height - 4);
        painter -> drawText(textRect, index.data().toString());

        textRect.setBottom(textRect.bottom() + height);
        textRect.setTop(textRect.top() + height);
        painter -> drawText(textRect, index.data(DOWNLOAD_ERROR).toString());

        painter -> save();

        QPen p(QColor::fromRgb(255, 0, 0));
        p.setWidth((option.state & QStyle::State_Selected) ? 3 : 1);
        painter -> setPen(p);
        painter -> drawRoundedRect(rect, 8, 8);
        painter -> drawLine(rect.left(), textRect.top() - 2, rect.right(), textRect.top() - 2);

        painter -> restore();
    } else {
        QStyledItemDelegate::paint(painter, option, index);
    }
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
