#include "relations_delegate.h"

QSize RelationsDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const {
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    return size + QSize(0, 30);
}

void RelationsDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    QRect r = option.rect;

    //Color: #C4C4C4
    QPen linePen(QColor::fromRgb(211, 211, 211), 1, Qt::SolidLine);

    //Color: #005A83
    QPen lineMarkedPen(QColor::fromRgb(0, 60, 111), 1, Qt::SolidLine);

    //Color: #333
    QPen fontPen(QColor::fromRgb(51, 51, 51), 1, Qt::SolidLine);

    //Color: #fff
    QPen fontMarkedPen(Qt::white, 1, Qt::SolidLine);

    if(option.state & QStyle::State_Selected){
        QLinearGradient gradientSelected(r.left(), r.top(), r.left(), r.height() + r.top());
        gradientSelected.setColorAt(0.0, QColor::fromRgb(119, 163, 197));
        gradientSelected.setColorAt(0.9, QColor::fromRgb(27, 134, 183));
        gradientSelected.setColorAt(1.0, QColor::fromRgb(0, 120, 174));
        painter -> setBrush(gradientSelected);
        painter -> drawRect(r);

        //BORDER
        painter -> setPen(lineMarkedPen);
        painter -> drawLine(r.topLeft(),r.topRight());
        painter -> drawLine(r.topRight(),r.bottomRight());
        painter -> drawLine(r.bottomLeft(),r.bottomRight());
        painter -> drawLine(r.topLeft(),r.bottomLeft());

        painter -> setPen(fontMarkedPen);

    } else {
        //BACKGROUND
        painter -> setBrush((index.row() % 2) ? Qt::white : QColor(240, 240, 240));
        painter -> drawRect(r);

        //BORDER
        painter -> setPen(linePen);
        painter -> drawLine(r.topLeft(), r.topRight());
        painter -> drawLine(r.topRight(), r.bottomRight());
        painter -> drawLine(r.bottomLeft(), r.bottomRight());
        painter -> drawLine(r.topLeft(), r.bottomLeft());

        painter -> setPen(fontPen);
    }

    //GET TITLE, DESCRIPTION AND ICON
    QIcon ic = QIcon(qvariant_cast<QPixmap>(index.data(Qt::DecorationRole)));
    QString title = index.data(Qt::DisplayRole).toString();
    QString description = index.data(Qt::UserRole + 1).toString();

    int imageSpace = 10;
    if (!ic.isNull()) {
        //ICON
        r = option.rect.adjusted(5, 10, -10, -10);
        ic.paint(painter, r, Qt::AlignVCenter | Qt::AlignLeft);
        imageSpace = 55;
    }

    //TITLE
    r = option.rect.adjusted(imageSpace, 0, -10, -30);
    painter -> setFont(QFont("Lucida Grande", 10, QFont::Bold));
    painter -> drawText(r.left(), r.top() + 5, r.width(), r.height(), Qt::AlignBottom | Qt::AlignLeft, title, &r);

    //DESCRIPTION
    r = option.rect.adjusted(imageSpace, 30, -10, 0);
    painter -> setFont(QFont("Lucida Grande", 9, QFont::Bold));
    painter -> drawText(r.left(), r.top() - 3, r.width(), r.height(), Qt::AlignLeft, description, &r);
}
