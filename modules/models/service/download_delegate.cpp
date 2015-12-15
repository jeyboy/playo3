#include "download_delegate.h"
#include "settings.h"

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
    bool isSaving = progressPercentage >= 0;

    if (!isSaving)
        progressPercentage = index.data(REMOTE_PROGRESS).toInt();

    painter -> setRenderHint(QPainter::Antialiasing, true);
    painter -> setRenderHint(QPainter::TextAntialiasing, true);

    QRect rect = option.rect;
    rect.setTopLeft(rect.topLeft() + QPoint(4, 1));
    rect.setBottomRight(rect.bottomRight() - QPoint(8, 2));

    if (progressPercentage >= 0) {
        QProgressBar renderer;

        renderer.setAttribute(Qt::WA_OpaquePaintEvent, true);
        renderer.setAttribute(Qt::WA_NoSystemBackground, true);
        renderer.setAttribute(Qt::WA_TranslucentBackground, true);

        Settings::currentStyle -> applyProperty(&renderer, "download",
            option.state & QStyle::State_Selected ? QStringLiteral("selected") : QStringLiteral("usual")
        );

        renderer.setTextVisible(true);
        QString str = (isSaving ? QStringLiteral("(Saving) ") : QStringLiteral("(Reading) ")) % index.model() -> data(index, Qt::DisplayRole).toString();
        renderer.setFormat(option.fontMetrics.elidedText(str, Qt::ElideRight, rect.width() - 8));
        renderer.resize(rect.size());
        renderer.setMinimum(0);
        renderer.setMaximum(100);
        renderer.setValue(progressPercentage);

        painter -> save();
        painter -> translate(rect.topLeft());
        renderer.render(painter);
        painter -> restore();
    } else if (progressPercentage == -2) {
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
        p.setWidth((option.state & QStyle::State_Selected) ? 2 : 1);
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
