#include "model_item_delegate.h"
#include <QDebug>

ModelItemDelegate::ModelItemDelegate(QObject * parent)
    : QStyledItemDelegate(parent),
    ico_mini(30) {

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

void ModelItemDelegate::recalcAttrs(int item_icon_size) {
    itemFont = Settings::instance() -> itemFont();
    itemInfoFont = Settings::instance() -> itemInfoFont();
    fmf = new QFontMetrics(itemFont);
    fmfInfo = new QFontMetrics(itemInfoFont);
    icon_size = item_icon_size - 2;
}

void ModelItemDelegate::drawCheckbox(bool is_container, QVariant & checkable, QPainter * painter, const QStyleOptionViewItem& option) const {
    QStyleOptionButton checkboxstyle;
    QRect rect = option.rect;
    rect.setLeft(rect.left() + (is_container ? 4 : -14));
    checkboxstyle.rect = rect;
    checkboxstyle.state = option.state;
    checkboxstyle.state |= checkable.toBool() ? QStyle::State_On : QStyle::State_Off;

    QApplication::style() -> drawControl(QStyle::CE_CheckBox, &checkboxstyle, painter, &templateCheckbox);
}

void ModelItemDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    painter -> save();
    painter -> setRenderHint(QPainter::Antialiasing, true);
    painter -> setRenderHint(QPainter::TextAntialiasing, true);

    QRect bodyRect = option.rect;
    bodyRect.setTop(bodyRect.top() + 1); // space between items
    bodyRect.setHeight(bodyRect.height() - 2);  // space between items

    QVariantMap attrs = index.data(IATTRS).toMap();
    QVariant checkable = attrs.value("checkable");

    int background_state = attrs.value("state").toInt();
    int angle = bodyRect.height() / 2, ico_offset = 0, right_offset = bodyRect.right() - 12, top = option.rect.bottom(), left_offset = bodyRect.left() + 10;
    bool is_folder = false, is_selected = option.state & (QStyle::State_Selected);

    QBrush state_color;

    switch (background_state) {
        case Playo3::ItemState::new_item:
            state_color = Settings::instance() -> defaultState(bodyRect, !is_selected);
            break;
        case Playo3::ItemState::listened:
            state_color = Settings::instance() -> listenedState(bodyRect, !is_selected);
            break;
        case Playo3::ItemState::liked:
            state_color = Settings::instance() -> likedState(bodyRect, !is_selected);
            break;
        case Playo3::ItemState::played:
            state_color = Settings::instance() -> playedState(bodyRect, !is_selected);
            break;
        default:
            is_folder = true;
            break;
    }

    QColor textColor = is_selected ? Settings::instance() -> selectedItemTextColor() : Settings::instance() -> itemTextColor(); // option.palette.color(QPalette::Dark)

    if (!is_folder) {
        left_offset += icon_size + ico_offset;

        bodyRect.moveLeft(bodyRect.left() + 2);
        bodyRect.setWidth(bodyRect.width() - 2);
    }

    painter -> setPen(textColor);
    painter -> setBrush(Settings::instance() -> unprocessedState(bodyRect, is_selected));
    painter -> drawRoundedRect(bodyRect, angle, angle);

    if (checkable.isValid()) {
        left_offset += is_folder ? 14 : 6;
        drawCheckbox(is_folder, checkable, painter, option);
    } else {
        left_offset += is_folder ? 0 : 6;
    }

    //    painter -> setClipRect(bodyRect);

    if (!is_folder) {
        QRect icoRect = QRect(bodyRect.left() + 2 + (icon_size / 20), option.rect.top() + (option.rect.height() - icon_size) / 2, icon_size, icon_size);

        if (Settings::instance() -> isShowSystemIcons()) {
            QVariant iconVal = attrs.value("icon");
            if (iconVal.isValid()) {
                QIcon icon = qvariant_cast<QIcon>(iconVal);
                painter -> drawPixmap(icoRect, icon.pixmap(QSize(icon_size, icon_size)));
            }
        } else {

        }

        if (attrs["type"] == VK_ITEM || attrs["type"] == SOUNDCLOUD_ITEM) {
            QFont font; font.setFamily("Arial Black"); font.setPixelSize(16);
            painter -> setFont(font);
            painter -> drawText(icoRect.topRight() + QPoint(3, 10), "*");
        }

        ///////////////////////////////////////////////////
        QPen cPen(state_color, 6); cPen.setCosmetic(true);
        painter -> setPen(cPen);
        painter -> drawEllipse(icoRect);
        ///////////////////////////////////////////////////

        if (Settings::instance() -> isShowInfo()) {
            painter -> setFont(itemInfoFont);
            painter -> setPen(
                        is_selected ?
                            Settings::instance() -> selectedItemInfoTextColor()
                          :
                            Settings::instance() -> itemInfoTextColor()
                        );

            QStringList infos = attrs.value("info").toStringList();

            int timeWidth = fmfInfo -> width(infos.last());
            int right_offset_with_corner = right_offset - (angle / 3);
            int beetweeX = right_offset_with_corner - timeWidth - 2;
            top = bodyRect.bottom() - fmfInfo -> height() - 2;

            QPoint topLeft(left_offset, top);
            QPoint bottomRight(beetweeX - 4, bodyRect.bottom());
            QRect rectText(topLeft, bottomRight);
            QString s = fmfInfo -> elidedText(infos.first(), Qt::ElideRight, rectText.width());

            QPoint topTLeft(beetweeX, top);
            QPoint bottomTRight(right_offset_with_corner, bodyRect.bottom());
            QRect rectTimeText(topTLeft, bottomTRight);

            painter -> drawText(rectText, Qt::AlignLeft, s);
            painter -> drawText(rectTimeText, Qt::AlignRight, infos.last());

            painter -> drawLine(rectText.topLeft(), rectTimeText.topRight());
        }
    }

    painter -> setFont(itemFont);
    painter -> setPen(textColor);

    QPoint topMLeft(left_offset, bodyRect.top() + 2);
    QPoint bottomMRight(right_offset, top - 2);

    QRect rectText2(topMLeft, bottomMRight);
    QString s = fmf -> elidedText(attrs.value("name").toString(), Qt::ElideRight, rectText2.width());
    painter -> drawText(rectText2, Qt::AlignLeft | Qt::AlignVCenter, s);

    if (option.state & (QStyle::State_MouseOver)) {
        painter -> setPen(hoverColor);
        painter -> setBrush(hoverColor);
        painter -> drawRoundedRect(bodyRect, angle, angle);
    }

    painter -> restore();
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

//void ModelItemDelegate::usuall(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
//    QColor textColor;
//    QStyleOptionViewItem option2 = option;
//    qDebug() << option;
//    option2.rect.setTop(option.rect.top() + 2);
//    option2.rect.setHeight(option.rect.height() - 4);

//    QVariantMap attrs = index.data(IATTRS).toMap();

//    QString ext = attrs.value("ext").toString();
//    int x, y, width, height, icon_size = 1, ico_mini = 30, ico_offset = 0, right_offset = option2.rect.right() - 12, top = option.rect.bottom(), left_offset = option2.rect.left() + 10;
//    option2.rect.getRect(&x, &y, &width, &height);

//    painter -> save();
//    painter -> setRenderHint(QPainter::Antialiasing, true);
//    painter -> setRenderHint(QPainter::TextAntialiasing, true);

//    int background_state = attrs.value("state").toInt();
//    QVariant checkable = index.data(Qt::CheckStateRole);

//    bool elem_state = option2.state & (QStyle::State_Selected);
//    bool is_folder = false;

//    QBrush fill_color;

//    switch (background_state) {
//        case Playo3::ItemState::new_item:
//            fill_color = Settings::instance() -> defaultState(option2.rect, elem_state);
//            break;
//        case Playo3::ItemState::listened:
//            fill_color = Settings::instance() -> listenedState(option2.rect, elem_state);
//            break;
//        case Playo3::ItemState::liked:
//            fill_color = Settings::instance() -> likedState(option2.rect, elem_state);
//            break;
//        case Playo3::ItemState::played:
//            fill_color = Settings::instance() -> playedState(option2.rect, elem_state);
//            break;
//        default:
//            is_folder = true;
//            fill_color = Settings::instance() -> unprocessedState(option2.rect, elem_state);
//            break;
//    }

//    if (elem_state)
//        painter -> setPen(Settings::instance() -> getSelectedItemTextColor());
//    else
//        painter -> setPen(option2.palette.color(QPalette::Dark));

//    painter -> setBrush(fill_color);
//    int angle = option2.rect.height() / 2.2;
//    QRect bodyRect = option2.rect;

//    if (!is_folder) {
//        icon_size = ((QTreeView *)option2.widget) -> iconSize().width() - 2;
//        if (icon_size < ico_mini && !Settings::instance() -> isShowSystemIcons()) {
//            ico_offset = 8 * (ext.length() - 1);
//        }

//        left_offset += icon_size + ico_offset;

//        int rectOffset = (checkable.isValid() ? 18 : 2);
//        bodyRect.moveLeft(bodyRect.left() + rectOffset);
//        bodyRect.setWidth(bodyRect.width() - rectOffset);
//    }

////    painter -> setClipRect(bodyRect);
//    painter -> drawRoundedRect(bodyRect, angle, angle);

//    if (checkable.isValid()) {
//        left_offset += 14;
//        drawCheckbox(painter, option2, index);
//    }

//    if(elem_state) {
//        textColor = Settings::instance() -> getSelectedItemTextColor();
//    } else {
//        textColor = Settings::instance() -> getItemTextColor();
//    }
//    painter -> setPen(textColor);

//    if (!is_folder) {
//        if (Settings::instance() -> isShowSystemIcons()) {
//            QRect icoRect = QRect(
//                        bodyRect.left() + 4 + (icon_size / 20),
//                        option.rect.top() + (option.rect.height() - icon_size) / 2,
//                        icon_size,
//                        icon_size
//                    );
//            QVariant iconVal = index.data(Qt::DecorationRole);

//            if (iconVal.isValid()) {
//                QIcon icon = qvariant_cast<QIcon>(iconVal);
//                painter -> drawPixmap(icoRect, icon.pixmap(QSize(icon_size, icon_size)));
//            }
//        } else {
//            QRect pseudoIcoRect = QRect(
//                        bodyRect.left(),
//                        option.rect.top() + 2,
//                        icon_size + (ico_offset + 4),
//                        bodyRect.height()
//                    );

//                    QFont font;
//                    font.setFamily("Arial Black");

//                    if (icon_size < ico_mini) {
//                        painter -> drawLine(pseudoIcoRect.topRight(), pseudoIcoRect.bottomRight());
//                    } else {
//            //            QPen pen(textColor);
//            //            pen.setWidth(2);
//            //            painter -> setPen(pen);
//                        if(!elem_state)
//                            painter -> setPen(option2.palette.color(QPalette::Dark));
//                        painter -> drawEllipse(pseudoIcoRect);
////                        painter -> drawArc(pseudoIcoRect, -75 * 16, 150 * 16);
////                        painter -> drawRoundedRect(pseudoIcoRect, angle, angle);
//                        painter -> setPen(textColor);

//                        if (ext.length() > 3) {
//                            ext.insert(3, '\n');
//                          if (ext.length() > 6)
//                            font.setPixelSize(icon_size / (ext.length() / 2));
//                        }
//                        painter -> setFont(font);
//                    }

//                    painter -> drawText(
//                                pseudoIcoRect,
//                                Qt::AlignHCenter | Qt::AlignVCenter,
//                                ext
//                            );
//        }
//    }

//    if (Settings::instance() -> isShowInfo() && !is_folder) {
//        QFont vfont = Settings::instance() -> getItemFont(); //index.data(IADDFONT).value<QFont>();
//        QStringList infos = attrs.value("info").toStringList();//index.model() -> data(index, IINFO).toStringList();

//        painter -> setFont(vfont);
//        QFontMetrics fmf(vfont);
//        int timeWidth = fmf.width(infos.last());

//        int beetweeX = right_offset - timeWidth - 2 - (angle / 3);
//        top = option2.rect.bottom() - fmf.height() - 2;

//        QPoint topLeft(left_offset, top);
//        QPoint bottomRight(beetweeX - 4, option2.rect.bottom());
//        QRect rectText(topLeft, bottomRight);
//        QString s = fmf.elidedText(infos.first(), Qt::ElideRight, rectText.width());

//        QPoint topTLeft(beetweeX, top);
//        QPoint bottomTRight(right_offset - (angle / 3), option2.rect.bottom());
//        QRect rectTimeText(topTLeft, bottomTRight);

//        painter -> drawText(rectText, Qt::AlignLeft, s);
//        painter -> drawText(rectTimeText, Qt::AlignRight, infos.last());

//        painter -> drawLine(rectText.topLeft(), rectTimeText.topRight());
//    }

//    painter -> setFont(Settings::instance() -> getItemFont());
//    QFontMetrics fmf2(painter -> font());
//    QPoint topMLeft(left_offset, option2.rect.top());
//    QPoint bottomMRight(right_offset, top - 2);

//    QRect rectText2(topMLeft, bottomMRight);
//    QString s = fmf2.elidedText(index.data().toString(), Qt::ElideRight, rectText2.width());
//    painter -> drawText(rectText2, Qt::AlignLeft | Qt::AlignVCenter, s);

//    if (option2.state & (QStyle::State_MouseOver)) {
//        painter -> setPen(hoverColor);
//        painter -> setBrush(hoverColor);
//        painter -> drawRoundedRect(bodyRect, angle, angle);
//    }

//    painter -> restore();
//}

