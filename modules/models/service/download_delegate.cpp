#include "download_delegate.h"

#include "settings.h"
#include "download_model_item.h"


QSize DownloadDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const {
    QSize size = QStyledItemDelegate::sizeHint(option, index);

    if (index.data(DOWNLOAD_ERROR).isValid()) {
        size.setHeight(size.height() * 2);
        return size;
    }
    else return size;
}

void DownloadDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    QRect rect = option.rect;

    if (index.data(DOWNLOAD_ERROR).isValid()) {
        rect.setTopLeft(rect.topLeft() + QPoint(1, 1));
        rect.setSize(rect.size() - QSize(2, 2));

        painter -> save();
        QRect textRect(rect);

        QLinearGradient grad(rect.left(), rect.top(), rect.left(), rect.bottom());
        grad.setColorAt(0, (option.state & QStyle::State_Selected) ? QColor::fromRgb(255, 32, 32) : Qt::red);
        grad.setColorAt(0.95, (option.state & QStyle::State_Selected) ? Qt::white : Qt::black);

        QPen p((option.state & QStyle::State_Selected) ? Qt::black : Qt::white);
        p.setWidth(2);
        painter -> setPen(p);
        painter -> setBrush(grad);
        painter -> drawRoundedRect(rect, 8, 8);

        textRect.setLeft(rect.left() + 4);
        textRect.setRight(rect.right() - 8);

        int height = textRect.height() / 2;

        textRect.setBottom(textRect.bottom() - height - 4);
        painter -> drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, index.data().toString());

        textRect.setBottom(textRect.bottom() + height);
        textRect.setTop(textRect.top() + height);
        painter -> drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, index.data(DOWNLOAD_ERROR).toString());
        painter -> drawLine(rect.left(), textRect.top() - 2, rect.right(), textRect.top() - 2);

        painter -> restore();
    } else {
        int progressPercentage = index.data(REMOTE_PROGRESS).toInt();
        bool isDownloading = progressPercentage >= 0;

        if (!isDownloading)
            progressPercentage = index.data(DOWNLOAD_PROGRESS).toInt();

        painter -> setRenderHint(QPainter::Antialiasing, true);
        painter -> setRenderHint(QPainter::TextAntialiasing, true);

        rect.setTopLeft(rect.topLeft() + QPoint(4, 1));
        rect.setBottomRight(rect.bottomRight() - QPoint(8, 2));

        if (progressPercentage >= 0) {
            QProgressBar renderer;

            renderer.setAttribute(Qt::WA_OpaquePaintEvent, true);
            renderer.setAttribute(Qt::WA_NoSystemBackground, true);
            renderer.setAttribute(Qt::WA_TranslucentBackground, true);

            Settings::currentStyle -> applyProperty(&renderer, "download",
                option.state & QStyle::State_Selected ? QLatin1String("selected") : QLatin1String("usual")
            );

            renderer.setTextVisible(true);
            QString str = (isDownloading ? QLatin1String("(Downloading) ") : QLatin1String("(Dump to disk) ")) % index.model() -> data(index, Qt::DisplayRole).toString();
            renderer.setFormat(option.fontMetrics.elidedText(str, Qt::ElideRight, rect.width() - 8));
            renderer.resize(rect.size());
            renderer.setMinimum(0);
            renderer.setMaximum(100);
            renderer.setValue(progressPercentage);

            painter -> save();
            painter -> translate(rect.topLeft());
            renderer.render(painter);
            painter -> restore();
        }
        else {
            QLinearGradient grad(rect.left(), rect.top(), rect.left(), rect.bottom());
            grad.setColorAt(0, QColor::fromRgb(0, 163, 255));
            grad.setColorAt(1, (option.state & QStyle::State_Selected) ? Qt::white : Qt::gray);

            QPen p((option.state & QStyle::State_Selected) ? Qt::black : Qt::white);
            painter -> setPen(p);
            painter -> setBrush(grad);

            painter -> drawRoundedRect(rect, 8, 8);
            rect.moveLeft(rect.left() + 8);
            painter -> drawText(rect, Qt::AlignLeft | Qt::AlignVCenter, option.fontMetrics.elidedText(index.data().toString(), Qt::ElideRight, rect.width() + 4));
//            QStyledItemDelegate::paint(painter, option, index);
        }
    }
}
