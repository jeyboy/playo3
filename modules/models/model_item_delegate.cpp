#include "model_item_delegate.h"
#include "external_keys.h"

using namespace Core::Web;

ModelItemDelegate::ModelItemDelegate(QObject * parent)
    : QStyledItemDelegate(parent),
    ico_mini(30), state_width(6) {

    hoverColor = QColor(Qt::white);
    hoverColor.setAlpha(80);

    extra_font_name = QStringLiteral("Arial Black");
    extra_font_size = 16;
}

//QSize ModelItemDelegate::sizeHint(const QStyleOptionViewItem & option, const QModelIndex & index) const {
//    QSize size = QStyledItemDelegate::sizeHint(option, index);
//    size.setHeight(size.height() + 6);
//    return size;

////    return QStyledItemDelegate::sizeHint(option, index);
//}

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
    itemFont = Settings::obj().itemFont();
    itemInfoFont = Settings::obj().itemInfoFont();
    fmf = new QFontMetrics(itemFont);
    fmfInfo = new QFontMetrics(itemInfoFont);
    icon_size = item_icon_size - 2;

    int size = icon_size - state_width * 2;

    icons.insert(-300,                                                  PIXMAP(QStringLiteral(":/items/warn"), size));
    icons.insert(-200,                                                  PIXMAP(QStringLiteral(":/items/process"), size));
    icons.insert(-200 + SELECTION_ITER,                                 PIXMAP(QStringLiteral(":/items/process_on"), size));
    icons.insert(-100,                                                  PIXMAP(QStringLiteral(":/items/err"), size));
    icons.insert(VK_FILE,                                               PIXMAP(QStringLiteral(":/items/vk_item"), size));
    icons.insert(VK_FILE + SELECTION_ITER,                              PIXMAP(QStringLiteral(":/items/vk_item_on"), size));
    icons.insert(SOUNDCLOUD_FILE,                                       PIXMAP(QStringLiteral(":/items/sc_item"), size));
    icons.insert(SOUNDCLOUD_FILE + SELECTION_ITER,                      PIXMAP(QStringLiteral(":/items/sc_item_on"), size));
    icons.insert(OD_FILE,                                               PIXMAP(QStringLiteral(":/items/od_item"), size));
    icons.insert(OD_FILE + SELECTION_ITER,                              PIXMAP(QStringLiteral(":/items/od_item_on"), size));
    icons.insert(WEB_FILE,                                              PIXMAP(QStringLiteral(":/items/web_item"), size));
    icons.insert(WEB_FILE + SELECTION_ITER,                             PIXMAP(QStringLiteral(":/items/web_item_on"), size));
    icons.insert(SIMPLE_FILE,                                           PIXMAP(QStringLiteral(":/items/local_item"), size));
    icons.insert(SIMPLE_FILE + SELECTION_ITER,                          PIXMAP(QStringLiteral(":/items/local_item_on"), size));
    icons.insert(CUE_FILE, icons[SIMPLE_FILE]);
    icons.insert(CUE_FILE + SELECTION_ITER, icons[SIMPLE_FILE + SELECTION_ITER]);
    icons.insert(WEB_FILE + fourshared_site,                            PIXMAP(QStringLiteral(":/items/fourshared_item"), size));
    icons.insert(WEB_FILE + fourshared_site + SELECTION_ITER,           PIXMAP(QStringLiteral(":/items/fourshared_item_on"), size));
    icons.insert(WEB_FILE + zaycev_site,                                PIXMAP(QStringLiteral(":/items/zaycev_item"), size));
    icons.insert(WEB_FILE + zaycev_site + SELECTION_ITER,               PIXMAP(QStringLiteral(":/items/zaycev_item_on"), size));
    icons.insert(WEB_FILE + promodj_site,                               PIXMAP(QStringLiteral(":/items/promodj_item"), size));
    icons.insert(WEB_FILE + promodj_site + SELECTION_ITER,              PIXMAP(QStringLiteral(":/items/promodj_item_on"), size));
    icons.insert(WEB_FILE + jetune_site,                                PIXMAP(QStringLiteral(":/items/jetune_item"), size));
    icons.insert(WEB_FILE + jetune_site + SELECTION_ITER,               PIXMAP(QStringLiteral(":/items/jetune_item_on"), size));

}

void ModelItemDelegate::drawCheckbox(bool is_container, QVariant & checkable, QPainter * painter, const QStyleOptionViewItem& option) const {
    QStyleOptionButton checkboxstyle;
    QRect rect = option.rect;
    rect.setLeft(rect.left() + (is_container ? 4 : -3));
    checkboxstyle.rect = rect;
    checkboxstyle.state = option.state;
    checkboxstyle.state |= checkable.toBool() ? QStyle::State_On : QStyle::State_Off;

    QApplication::style() -> drawControl(QStyle::CE_CheckBox, &checkboxstyle, painter, &templateCheckbox);
}

void ModelItemDelegate::paintVar1(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    painter -> save();
    painter -> setRenderHint(QPainter::Antialiasing, true);
    painter -> setRenderHint(QPainter::TextAntialiasing, true);

    QRect bodyRect = option.rect;
    bodyRect.setTop(bodyRect.top() + 1); // space between items
    bodyRect.setHeight(bodyRect.height() - 2);  // space between items

    QVariantMap attrs = index.data(IATTRS).toMap();
    QVariant checkable = attrs.value(Key::checkable);

    QString ext;
    int angle = bodyRect.height() / 2.2, ico_offset = 0, right_offset = bodyRect.right() - 12, top = option.rect.bottom(), left_offset = bodyRect.left() + 10;
    int background_state = attrs.value(Key::state).toInt();
    bool is_folder = false, is_selected = option.state & (QStyle::State_Selected);

    QBrush fill_color;

    if (attrs.value(Key::played).toBool())
        fill_color = Settings::obj().playedState(bodyRect, is_selected);
    else {
        switch (background_state) {
            case Core::ItemState::new_item:
                fill_color = Settings::obj().defaultState(bodyRect, is_selected);
                break;
            case Core::ItemState::listened:
                fill_color = Settings::obj().listenedState(bodyRect, is_selected);
                break;
            case Core::ItemState::liked:
                fill_color = Settings::obj().likedState(bodyRect, is_selected);
                break;
            default:
                is_folder = true;
                fill_color = Settings::obj().unprocessedState(bodyRect, is_selected);
        }
    }

    QColor textColor = is_selected ? Settings::obj().selectedItemTextColor() : Settings::obj().itemTextColor();
    painter -> setBrush(fill_color);

    if (!is_folder) {
        if (!Settings::obj().isShowSystemIcons())
            ext = attrs.value(Key::ext).toString();

        if (icon_size < ico_mini && !Settings::obj().isShowSystemIcons())
            ico_offset = 8 * (ext.length() - 1);

        left_offset += icon_size + ico_offset;

        int rectOffset = (checkable.isValid() ? 18 : 2);
        bodyRect.moveLeft(bodyRect.left() + rectOffset);
        bodyRect.setWidth(bodyRect.width() - rectOffset);
    }

//    painter -> setClipRect(bodyRect);
    painter -> setPen(Qt::NoPen);
    painter -> drawRoundedRect(bodyRect, angle, angle);
    painter -> setPen(textColor);

    if (checkable.isValid())
        left_offset += 14;

    if (!is_folder) {
        QRect rect = QRect(
                    bodyRect.left(),
                    option.rect.top() + 1,
                    icon_size + (ico_offset + 4),
                    bodyRect.height()
                );

        if (attrs[Key::undefined].toBool())
            painter -> drawPixmap(rect, icons[-300]);
        else if (attrs[Key::proccessing].toBool())
            painter -> drawPixmap(rect, icons[-200 + (is_selected ? SELECTION_ITER : 0)]);
        else {
            if (attrs[Key::not_exist].toBool())
                painter -> drawPixmap(rect, icons[-100]);
            else {
                if (Settings::obj().isShowSystemIcons()) {
                    QRect icoRect = QRect(
                                bodyRect.left() + 4 + (icon_size / 20),
                                option.rect.top() + (option.rect.height() - icon_size) / 2,
                                icon_size,
                                icon_size
                            );
                    QVariant iconVal = attrs.value(Key::icon);

                    if (iconVal.isValid()) {
                        QIcon icon = qvariant_cast<QIcon>(iconVal);
                        painter -> drawPixmap(icoRect, icon.pixmap(QSize(icon_size, icon_size)));
                    }
                } else {
                    QRect pseudoIcoRect = QRect(
                                bodyRect.left(),
                                option.rect.top() + 1,
                                icon_size + (ico_offset + 4),
                                bodyRect.height()
                            );

                    QFont font;
                    font.setFamily(extra_font_name);

                    if (icon_size < ico_mini) {
                        painter -> drawLine(pseudoIcoRect.topRight(), pseudoIcoRect.bottomRight());
                    } else {
                        if(!is_selected)
                            painter -> setPen(option.palette.color(QPalette::Dark));
                        painter -> drawEllipse(pseudoIcoRect);
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

                    if (attrs[Key::type] == VK_FILE || attrs[Key::type] == SOUNDCLOUD_FILE) {
                        font.setPixelSize(extra_font_size);
                        painter -> setFont(font);

                        painter -> drawText(
                            pseudoIcoRect.topRight() + QPoint(-10, 13),
                            Key::asterix
                        );
                    }
                }
            }
        }
    }

    if (!is_folder && Settings::obj().isShowInfo()) {
        painter -> setFont(itemInfoFont);
        painter -> setPen(
                    is_selected ?
                        Settings::obj().selectedItemInfoTextColor()
                      :
                        Settings::obj().itemInfoTextColor()
                    );

        QStringList infos = attrs.value(Key::info).toStringList();

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

    painter -> setFont(itemFont);
    painter -> setPen(textColor);

    QPoint topMLeft(left_offset, bodyRect.top() + 2);
    QPoint bottomMRight(right_offset, top - 2);

    QRect rectText2(topMLeft, bottomMRight);
    QString s = fmf -> elidedText(attrs.value(Key::name).toString(), Qt::ElideRight, rectText2.width());
    painter -> drawText(rectText2, Qt::AlignLeft | Qt::AlignVCenter, s);

//    if (is_folder) {
//        painter -> setPen(textColor);
//        int y = rectText2.center().y();
//        painter -> drawLine(bodyRect.left() + fmf -> width(s) + 30, y, bodyRect.right() - 10, y);
//    }

    if (option.state & (QStyle::State_MouseOver)) {
        painter -> setPen(hoverColor);
        painter -> setBrush(hoverColor);
        painter -> drawRoundedRect(bodyRect, angle, angle);
    }

    if (checkable.isValid())
        drawCheckbox(is_folder, checkable, painter, option);

    painter -> restore();
}

void ModelItemDelegate::paintVar2(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const {
    painter -> save();
    painter -> setRenderHint(QPainter::Antialiasing, true);
    painter -> setRenderHint(QPainter::TextAntialiasing, true);

    QRect bodyRect = option.rect;
    bodyRect.setTop(bodyRect.top() + 1); // space between items
    bodyRect.setHeight(bodyRect.height() - 2);  // space between items

    QVariantMap attrs = index.data(IATTRS).toMap();
    QVariant checkable = attrs.value(Key::checkable);

    int background_state = attrs.value(Key::state).toInt();
    int angle = bodyRect.height() / 2, right_offset = bodyRect.right() - 12, top = option.rect.bottom(), left_offset = bodyRect.left() + 4;
    bool is_folder = false, is_selected = option.state & (QStyle::State_Selected);

    QBrush state_color;

    switch (background_state) {
        case Core::ItemState::new_item:
            state_color = Settings::obj().defaultState(bodyRect, !is_selected);
            break;
        case Core::ItemState::listened:
            state_color = Settings::obj().listenedState(bodyRect, !is_selected);
            break;
        case Core::ItemState::liked:
            state_color = Settings::obj().likedState(bodyRect, !is_selected);
            break;
        default: is_folder = true;
    }

    QColor textColor = is_selected ? Settings::obj().selectedItemTextColor() : Settings::obj().itemTextColor();

    if (!is_folder) {
        left_offset += icon_size;

        bodyRect.moveLeft(bodyRect.left() + 2);
        bodyRect.setWidth(bodyRect.width() - 2);
    }

    if (option.state & (QStyle::State_MouseOver))
        painter -> setPen(QPen(QBrush(textColor), 3));
    else
        painter -> setPen(Qt::NoPen);
    painter -> setBrush(
        attrs.value("played").toBool() ?
            Settings::obj().playedState(bodyRect, is_selected)
        :
            is_folder ?
                Settings::obj().unprocessedState(bodyRect, is_selected)
            :
                Settings::obj().itemState(bodyRect, is_selected)
    );
    painter -> drawRoundedRect(bodyRect, angle, angle);

    left_offset += is_folder ? (checkable.isValid() ? 14 : 0) : 6;

    //    painter -> setClipRect(bodyRect);

    if (!is_folder) {
        bool is_vk = attrs[Key::type] == VK_FILE, is_sc = attrs[Key::type] == SOUNDCLOUD_FILE;

        QRect icoRect = QRect(bodyRect.left() + 2 + (icon_size / 20), option.rect.top() + (option.rect.height() - icon_size) / 2, icon_size, icon_size);
        QRect rect(icoRect.left() + state_width, option.rect.top() + state_width * 1.5 + icon_size % 2, icon_size - state_width * 2, icon_size - state_width * 2);
        painter -> setBrush(Qt::NoBrush);

        if (attrs[Key::undefined].toBool())
            painter -> drawPixmap(rect, icons[-300]);
        else if (attrs[Key::proccessing].toBool()) {
            painter -> drawPixmap(rect, icons[-200 + (is_selected ? SELECTION_ITER : 0)]);
        } else {
            if (attrs[Key::not_exist].toBool()) {
                painter -> drawPixmap(rect, icons[-100]);
            } else {
                if (Settings::obj().isShowSystemIcons()) {
                    QVariant iconVal = attrs.value(Key::icon);
                    if (iconVal.isValid()) {
                        QIcon icon = qvariant_cast<QIcon>(iconVal);
                        painter -> drawPixmap(rect, icon.pixmap(QSize(icon_size, icon_size)));
                    }
                } else {
                    if (icon_size > 24) {
                        int selection = (is_selected ? SELECTION_ITER : 0);
                        painter -> drawPixmap(rect, icons.value(attrs[Key::type].toInt() + selection, icons[WEB_FILE + selection]));
                    }
                }
            }
        }

        ///////////////////////////////////////////////////
        if (icon_size > 24) {
            QPen cPen(state_color, state_width); cPen.setCosmetic(true); cPen.setCapStyle(Qt::SquareCap);
            painter -> setPen(cPen);
        } else {
            painter -> setPen(Qt::NoPen);
            painter -> setBrush(state_color);
        }

//        painter -> drawArc(icoRect, 120 * 16, 70 * 16);
//        painter -> drawArc(icoRect, 235 * 16, 70 * 16);
//        painter -> drawArc(icoRect, 60 * 16, -70 * 16);
        painter -> drawEllipse(icoRect);
        ///////////////////////////////////////////////////

        if (is_vk || is_sc) {

            QFont font; font.setFamily(extra_font_name); font.setPixelSize(extra_font_size);
            painter -> setFont(font);
            painter -> setPen(Qt::NoPen);
            painter -> setBrush(QBrush(is_selected ? Settings::obj().itemTextColor() : Settings::obj().selectedItemTextColor()));
            QPointF centerP = icoRect.topLeft() + QPointF(icoRect.width() / 2 + 0.5, 1);
            int rect_size = extra_font_size / 4;
            painter -> drawEllipse(centerP, rect_size, rect_size);
            painter -> setPen(textColor);
            painter -> drawText(centerP + QPointF(-rect_size, 9), Key::asterix);
        }

        if (Settings::obj().isShowInfo()) {
            painter -> setFont(itemInfoFont);
            painter -> setPen(
                        is_selected ?
                            Settings::obj().selectedItemInfoTextColor()
                          :
                            Settings::obj().itemInfoTextColor()
                        );

            QStringList infos = attrs.value(Key::info).toStringList();

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
    QString s = fmf -> elidedText(attrs.value(Key::name).toString(), Qt::ElideRight, rectText2.width());
    painter -> drawText(rectText2, Qt::AlignLeft | Qt::AlignVCenter, s);

//    if (is_folder) {
//        painter -> setPen(textColor);
//        int y = rectText2.center().y();
//        painter -> drawLine(bodyRect.left() + fmf -> width(s) + 30, y, bodyRect.right() - 10, y);
//    }

//    if (option.state & (QStyle::State_MouseOver)) {
////        painter -> setPen(hoverColor);
////        painter -> setBrush(hoverColor);
//        painter -> drawRoundedRect(bodyRect, angle, angle);
//    }

    if (checkable.isValid())
        drawCheckbox(is_folder, checkable, painter, option);

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
