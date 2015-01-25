#include "model_item_delegate.h"
#include <QDebug>

ModelItemDelegate::ModelItemDelegate(QObject * parent)
    : QStyledItemDelegate(parent) {

    hoverColor = QColor(Qt::white);
    hoverColor.setAlpha(80);
}

QSize ModelItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const {
    QSize size = QStyledItemDelegate::sizeHint(option, index);
    size.setHeight(size.height() + 6);
    return size;

//    return QStyledItemDelegate::sizeHint(option, index);
}

//QWidget * ModelItemDelegate::createEditor(QWidget *parent,
//                      const QStyleOptionViewItem &option,
//                      const QModelIndex &index) const {

//}

//void ModelItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {

//}
//void ModelItemDelegate::setModelData(QWidget *editor,
//                  QAbstractItemModel *model,
//                  const QModelIndex &index) const;

//void ModelItemDelegate::updateEditorGeometry(QWidget *editor,
//                          const QStyleOptionViewItem &option,
//                          const QModelIndex &index) const;




//void ModelItemDelegate::usuall(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
//    //  //////////////////// remove focus rect
//    //  QStyleOptionViewItem option2 = option;
//    //  option2.rect.moveLeft(option2.rect.left() + offset);
//    //  option2.state = option.state & (~QStyle::State_HasFocus) & (~QStyle::State_Active) & (~QStyle::State_Selected);
//    //  /////////////////////////////////////////////
//    //  QStyledItemDelegate::paint(painter, option2, index);


//    //  if (index.isValid()) {
//    //    painter->setPen(Qt::SolidLine);
//    //    painter->setPen(QColor(Qt::lightGray));

//    //    painter->drawLine(QLine(option.rect.bottomLeft(), option.rect.bottomRight()));
//    //    painter->drawLine(QLine(option.rect.topRight(), option.rect.bottomRight()));
//    //  }

//        int iconSize = 16;
//        int delta = (option.state & QStyle::State_MouseOver) ? 32 : 0;
//        QColor semiTransparentWhite(255, 255, 255, 48 + delta);
//        QColor semiTransparentBlack(0, 0, 0, 48 - delta);

//        int x, y, width, height, offset, padding = 5;
//        option.rect.getRect(&x, &y, &width, &height);

//        int radius = qMin(width, height) / 1.5;

//        painter -> save();
//        painter -> setRenderHint(QPainter::Antialiasing, true);
//        int background_state = index.data(Qt::UserRole).toInt();

//        if (background_state != STATE_UNPROCESSED) {
//            offset = iconSize - 6;
//        } else {
//            offset = 5;
//        }

//        QPainterPath roundRect = roundRectPath(option.rect, offset);

//        bool elem_state = option.state & (QStyle::State_Selected | QStyle::State_HasFocus);

//        QLinearGradient fill_color;

//        switch (background_state) {
//            case STATE_DEFAULT:
//                fill_color = defaultState(option.rect, elem_state);
//                break;
//            case STATE_LISTENED:
//                fill_color = listenedState(option.rect, elem_state);
//                break;
//            case STATE_LIKED:
//                fill_color = likedState(option.rect, elem_state);
//                break;
//            case STATE_PLAYED:
//                fill_color = playedState(option.rect, elem_state);
//                break;
//            default:
//                fill_color = unprocessedState(option.rect, elem_state);
//                break;
//        }

//        painter -> fillPath(roundRect, fill_color);

//    //  if (option.state & (QStyle::State_Selected | QStyle::State_HasFocus))
//    //      painter->fillPath(roundRect, semiTransparentBlack);

//        int penWidth;
//        if (radius < 10)
//            penWidth = 3;
//        else if (radius < 20)
//            penWidth = 5;
//        else
//            penWidth = 7;

//        QPen topPen(semiTransparentWhite, penWidth);
//        QPen bottomPen(semiTransparentBlack, penWidth);

//        if (option.state & (QStyle::State_Sunken | QStyle::State_On))
//            qSwap(topPen, bottomPen);

//        int x1 = x;
//        int x2 = x + radius;
//        int x3 = x + width - radius;
//        int x4 = x + width;

//        if (option.direction == Qt::RightToLeft) {
//            qSwap(x1, x4);
//            qSwap(x2, x3);
//        }

//        QPolygon topHalf;
//        topHalf << QPoint(x1, y)
//                << QPoint(x4, y)
//                << QPoint(x3, y + radius)
//                << QPoint(x2, y + height - radius)
//                << QPoint(x1, y + height);

//        painter -> setClipPath(roundRect);
//        painter -> setClipRegion(topHalf, Qt::IntersectClip);
//        painter -> setPen(topPen);
//        painter -> drawPath(roundRect);

//        QPolygon bottomHalf = topHalf;
//        bottomHalf[0] = QPoint(x4, y + height);

//        painter -> setClipPath(roundRect);
//        painter -> setClipRegion(bottomHalf, Qt::IntersectClip);
//        painter -> setPen(bottomPen);
//        painter -> drawPath(roundRect);

//        painter -> setPen(option.palette.foreground().color());
//        painter -> setClipping(false);
//        painter -> drawPath(roundRect);

//        drawCheckbox(painter, option, index);

//        QVariant vfont = index.data(Qt::FontRole);
//        QIcon icon;
//        QVariant iconVal = index.model() -> data(index, Qt::DecorationRole);

//        if (iconVal.isValid()) {
//            QPoint topLeft(x - 7, option.rect.y() + 1);
//            QPoint bottomRight(topLeft.rx() + iconSize - 1, option.rect.bottom() - 1);

//            icon =  qvariant_cast<QIcon>(iconVal);
//    //        icon.paint(painter, QRect(topLeft, bottomRight));

//            painter -> drawPixmap(QRect(topLeft, bottomRight), icon.pixmap(QSize(16,16)), QRect(0, 0, 16, 16));
//        }

//        QPoint topLeft(x + offset + 15 + padding, option.rect.y());
//        QPoint bottomRight(option.rect.right() - padding, option.rect.bottom());
//        QRect rectText(topLeft, bottomRight);
//        QString s;

//        if (vfont.isValid()) {
//            QFont font = vfont.value<QFont>();
//            painter -> setFont(font);
//            QFontMetrics fmf(font);
//            s = fmf.elidedText(index.data().toString(), option.textElideMode, rectText.width());
//        } else {
//            s = option.fontMetrics.elidedText(index.data().toString(), option.textElideMode, rectText.width());
//        }

//        if (elem_state)
//            painter -> setPen(Qt::white);
//        painter -> drawText(rectText, Qt::AlignLeft, s);

//        painter -> restore();
//}

// last version
//void ModelItemDelegate::usuall(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {

////  //////////////////// remove focus rect
//      QStyleOptionViewItem option2 = option;
//      option2.rect.setTop(option2.rect.top() + 2);
//      option2.rect.setHeight(option2.rect.height() - 2);
//      option2.state = option.state & (~QStyle::State_HasFocus) & (~QStyle::State_Active) & (~QStyle::State_Selected);
////  /////////////////////////////////////////////

//    int x, y, width, height, right_offset = 12;
//    option.rect.getRect(&x, &y, &width, &height);

//    painter -> save();
//    painter -> setRenderHint(QPainter::Antialiasing, true);
//    painter -> setRenderHint(QPainter::TextAntialiasing, true);

//    int background_state = index.data(Qt::UserRole).toInt();
//    QVariant checkable = index.data(Qt::CheckStateRole);

//    bool elem_state = option.state & (QStyle::State_Selected);
//    bool is_folder = false;

//    QBrush fill_color;

//    switch (background_state) {
//        case STATE_DEFAULT:
//            fill_color = Settings::instance() -> defaultState(option.rect, elem_state);
//            break;
//        case STATE_LISTENED:
//            fill_color = Settings::instance() -> listenedState(option.rect, elem_state);
//            break;
//        case STATE_LIKED:
//            fill_color = Settings::instance() -> likedState(option.rect, elem_state);
//            break;
//        case STATE_PLAYED:
//            fill_color = Settings::instance() -> playedState(option.rect, elem_state);
//            break;
//        default:
//            is_folder = true;
//            fill_color = Settings::instance() -> unprocessedState(option.rect, elem_state);
////                painter -> setPen(Qt::SolidLine);
////                painter -> setPen(QColor(Qt::lightGray));

////                painter -> drawLine(QLine(option.rect.bottomLeft(), option.rect.bottomRight()));
////                painter -> drawLine(QLine(option.rect.topRight(), option.rect.bottomRight()));
//            break;
//    }

//    painter -> setPen(QColor(Qt::gray));
//    painter -> setBrush(fill_color);
//    int angle = option.rect.height() / 2.2;
//    QRect bodyRect = option2.rect;

//    if (!is_folder) {
//        int rectOffset = (checkable.isValid() ? 18 : 2);
//        bodyRect.moveLeft(bodyRect.left() + rectOffset);
//        bodyRect.setWidth(bodyRect.width() - rectOffset);
//    }

//    painter -> drawRoundedRect(bodyRect, angle, angle);

////    QPainterPath roundRect = roundRectPath(option.rect, !checkable.isValid() ? 2 : 18);
////    painter -> fillPath(roundRect, fill_color);

////    painter -> setPen(QColor(Qt::gray));
////    painter -> drawPath(roundRect);
//    painter -> setPen(option.palette.foreground().color());

//    if(elem_state) {
//        option2.palette.setColor(QPalette::Text, Settings::instance() -> getSelectedItemTextColor());
//    } else {
//        option2.palette.setColor(QPalette::Text, Settings::instance() -> getItemTextColor());
//    }

//    if (Settings::instance() -> isShowInfo() && !is_folder) {
//        QFont vfont = index.data(ADDFONTID).value<QFont>();
//        QStringList infos = index.model() -> data(index, INFOID).toStringList();

//        painter -> setFont(vfont);
//        QFontMetrics fmf(vfont);
//        int timeWidth = fmf.width(infos.last());

//        int beetweeX = option.rect.right() - timeWidth - right_offset - 2;
//        int top = option.rect.bottom() - fmf.height() - 2;

//        int icon_width = ((QTreeView *)option2.widget) -> iconSize().width();
//        if (Settings::instance() -> isCheckboxShow())
//            icon_width += 14;

//        QPoint topLeft(option.rect.x() + icon_width + 14, top);
//        QPoint bottomRight(beetweeX - 4, option.rect.bottom());
//        QRect rectText(topLeft, bottomRight);
//        QString s = fmf.elidedText(infos.first(), option.textElideMode, rectText.width());

//        QPoint topTLeft(beetweeX, top);
//        QPoint bottomTRight(option.rect.right() - right_offset, option.rect.bottom());
//        QRect rectTimeText(topTLeft, bottomTRight);

//        painter -> setPen(option2.palette.color(QPalette::Text));

//        if(elem_state) {
//            painter -> setPen(Settings::instance() -> getSelectedItemInfoTextColor());
//        } else {
//            painter -> setPen(Settings::instance() -> getItemInfoTextColor());
//        }

//        painter -> drawText(rectText, Qt::AlignLeft, s);
//        painter -> drawText(rectTimeText, Qt::AlignRight, infos.last());
//    }

//    painter -> restore();

//    if (!checkable.isValid())
//        option2.rect.moveLeft(option2.rect.left() + 4);

//    if (is_folder) {
//        option2.textElideMode = Qt::ElideLeft;
//    }

//    QStyledItemDelegate::paint(painter, option2, index);
//}

void ModelItemDelegate::usuall(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    QColor textColor;
    QStyleOptionViewItem option2 = option;
    option2.rect.setTop(option.rect.top() + 2);
    option2.rect.setHeight(option.rect.height() - 4);

    QString ext = index.data(EXTENSIONID).toString();
    int x, y, width, height, icon_size = 1, ico_mini = 30, ico_offset = 0, right_offset = option2.rect.right() - 12, top = option.rect.bottom(), left_offset = option2.rect.left() + 10;
    option2.rect.getRect(&x, &y, &width, &height);

    painter -> save();
    painter -> setRenderHint(QPainter::Antialiasing, true);
    painter -> setRenderHint(QPainter::TextAntialiasing, true);

    int background_state = index.data(STATEID).toInt();
    QVariant checkable = index.data(Qt::CheckStateRole);

    bool elem_state = option2.state & (QStyle::State_Selected);
    bool is_folder = false;

    QBrush fill_color;

    switch (background_state) {
        case Playo3::ItemState::new_item:
            fill_color = Settings::instance() -> defaultState(option2.rect, elem_state);
            break;
        case Playo3::ItemState::listened:
            fill_color = Settings::instance() -> listenedState(option2.rect, elem_state);
            break;
        case Playo3::ItemState::liked:
            fill_color = Settings::instance() -> likedState(option2.rect, elem_state);
            break;
        case Playo3::ItemState::played:
            fill_color = Settings::instance() -> playedState(option2.rect, elem_state);
            break;
        default:
            is_folder = true;
            fill_color = Settings::instance() -> unprocessedState(option2.rect, elem_state);
            break;
    }

    if (elem_state)
        painter -> setPen(Settings::instance() -> getSelectedItemTextColor());
    else
        painter -> setPen(option2.palette.color(QPalette::Dark));
    painter -> setBrush(fill_color);
    int angle = option2.rect.height() / 2.2;
    QRect bodyRect = option2.rect;

    if (!is_folder) {
        icon_size = ((QTreeView *)option2.widget) -> iconSize().width() - 2;
        if (icon_size < ico_mini && !Settings::instance() -> isShowSystemIcons()) {
            ico_offset = 8 * (ext.length() - 1);
        }

        left_offset += icon_size + ico_offset;

        int rectOffset = (checkable.isValid() ? 18 : 2);
        bodyRect.moveLeft(bodyRect.left() + rectOffset);
        bodyRect.setWidth(bodyRect.width() - rectOffset);
    }

//    painter -> setClipRect(bodyRect);
    painter -> drawRoundedRect(bodyRect, angle, angle);

    if (checkable.isValid()) {
        left_offset += 14;
        drawCheckbox(painter, option2, index);
    }

    if(elem_state) {
        textColor = Settings::instance() -> getSelectedItemTextColor();
    } else {
        textColor = Settings::instance() -> getItemTextColor();
    }
    painter -> setPen(textColor);

    if (!is_folder) {
        if (Settings::instance() -> isShowSystemIcons()) {
            QRect icoRect = QRect(
                        bodyRect.left() + 4 + (icon_size / 20),
                        option.rect.top() + (option.rect.height() - icon_size) / 2,
                        icon_size,
                        icon_size
                    );
            QVariant iconVal = index.data(Qt::DecorationRole);

            if (iconVal.isValid()) {
                QIcon icon = qvariant_cast<QIcon>(iconVal);
                painter -> drawPixmap(icoRect, icon.pixmap(QSize(icon_size, icon_size)));
            }
        } else {
            QRect pseudoIcoRect = QRect(
                        bodyRect.left(),
                        option.rect.top() + 2,
                        icon_size + (ico_offset + 4),
                        bodyRect.height()
                    );

                    QFont font;
                    font.setFamily("Arial Black");

                    if (icon_size < ico_mini) {
                        painter -> drawLine(pseudoIcoRect.topRight(), pseudoIcoRect.bottomRight());
                    } else {
            //            QPen pen(textColor);
            //            pen.setWidth(2);
            //            painter -> setPen(pen);
                        if(!elem_state)
                            painter -> setPen(option2.palette.color(QPalette::Dark));
                        painter -> drawEllipse(pseudoIcoRect);
//                        painter -> drawArc(pseudoIcoRect, -75 * 16, 150 * 16);
//                        painter -> drawRoundedRect(pseudoIcoRect, angle, angle);
                        painter -> setPen(textColor);

                        if (ext.length() > 3) {
                            ext.insert(3, '\n');
                          if (ext.length() > 6)
                            font.setPixelSize(icon_size / (ext.length() / 2));
                        }
                        painter -> setFont(font);
                    }

                    painter -> drawText(
                                pseudoIcoRect,
                                Qt::AlignHCenter | Qt::AlignVCenter,
                                ext
                            );
        }
    }

    if (Settings::instance() -> isShowInfo() && !is_folder) {
        QFont vfont = index.data(ADDFONTID).value<QFont>();
        QStringList infos = index.model() -> data(index, INFOID).toStringList();

        painter -> setFont(vfont);
        QFontMetrics fmf(vfont);
        int timeWidth = fmf.width(infos.last());

        int beetweeX = right_offset - timeWidth - 2 - (angle / 3);
        top = option2.rect.bottom() - fmf.height() - 2;

        QPoint topLeft(left_offset, top);
        QPoint bottomRight(beetweeX - 4, option2.rect.bottom());
        QRect rectText(topLeft, bottomRight);
        QString s = fmf.elidedText(infos.first(), Qt::ElideRight, rectText.width());

        QPoint topTLeft(beetweeX, top);
        QPoint bottomTRight(right_offset - (angle / 3), option2.rect.bottom());
        QRect rectTimeText(topTLeft, bottomTRight);

        painter -> drawText(rectText, Qt::AlignLeft, s);
        painter -> drawText(rectTimeText, Qt::AlignRight, infos.last());

        painter -> drawLine(rectText.topLeft(), rectTimeText.topRight());
    }

    painter -> setFont(Settings::instance() -> getItemFont());
    QFontMetrics fmf2(painter -> font());
    QPoint topMLeft(left_offset, option2.rect.top());
    QPoint bottomMRight(right_offset, top - 2);

    QRect rectText2(topMLeft, bottomMRight);
    QString s = fmf2.elidedText(index.data().toString(), Qt::ElideRight, rectText2.width());
    painter -> drawText(rectText2, Qt::AlignLeft | Qt::AlignVCenter, s);

    if (option2.state & (QStyle::State_MouseOver)) {
        painter -> setPen(hoverColor);
        painter -> setBrush(hoverColor);
        painter -> drawRoundedRect(bodyRect, angle, angle);
    }

    painter -> restore();
}

void ModelItemDelegate::drawCheckbox(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const {
    QStyleOptionButton checkboxstyle;
    bool checked = index.data(Qt::CheckStateRole).toBool();
    QRect rect = option.rect;
    rect.setLeft(rect.left() + 4);
    checkboxstyle.rect = rect;
    checkboxstyle.state = option.state;
    checkboxstyle.state |= checked ? QStyle::State_On : QStyle::State_Off;

    QApplication::style() -> drawControl(QStyle::CE_CheckBox, &checkboxstyle, painter, &templateCheckbox);
}

void ModelItemDelegate::progress(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index, int progressPercentage) const {
    painter -> save();
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
    painter -> restore();
}


void ModelItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    int progressPercentage = index.data(PROGRESSID).toInt();

    if (progressPercentage != -1) {
        progress(painter, option, index, progressPercentage);
    } else {
        usuall(painter, option, index);
    }
}

//QPainterPath ModelItemDelegate::backroundRectPath(const QRect &rect, int offset) {
//    int radius = qMin(rect.width(), rect.height()) / 2;
//    int diam = radius * 1.5, arc = radius;

//    int x1, y1, x2, y2;
//    rect.getCoords(&x1, &y1, &x2, &y2);
//    x1 += offset;

//    QPainterPath path;
//    path.moveTo(x2, y1 + radius);
//    path.arcTo(QRect(x2 - diam, y1, diam, diam), 0.0, +90.0);
//    path.arcTo(QRect(x1 + arc, y1, diam, diam), 90.0, -90.0);
//    path.arcTo(QRect(x1 + arc, y2 - diam, diam, diam), 0.0, -90.0);
//    path.arcTo(QRect(x2 - diam, y2 - diam, diam, diam), 270.0, +90.0);
//    path.closeSubpath();
//    return path;
//}


//QPainterPath ModelItemDelegate::roundRectPath(const QRect &rect, int offset) {
//    int radius = qMin(rect.width(), rect.height()) / 2;
//    int diam = radius * 1.5;

//    int x1, y1, x2, y2;
//    rect.getCoords(&x1, &y1, &x2, &y2);
//    x1 += offset;

//    QPainterPath path;
//    path.moveTo(x2, y1 + radius);
//    path.arcTo(QRect(x2 - diam, y1, diam, diam), 0.0, +90.0);
////    path.lineTo(x1 + radius, y1);
//    path.arcTo(QRect(x1, y1, diam, diam), 90.0, +90.0);
////    path.lineTo(x1, y2 - radius);
//    path.arcTo(QRect(x1, y2 - diam, diam, diam), 180.0, +90.0);
////    path.lineTo(x1 + radius, y2);
//    path.arcTo(QRect(x2 - diam, y2 - diam, diam, diam), 270.0, +90.0);
//    path.closeSubpath();
//    return path;
//}
