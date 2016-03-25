#include "model_item_delegate.h"
#include "external_keys.h"

using namespace Core;

ModelItemDelegate::ModelItemDelegate(QObject * parent)
    : QStyledItemDelegate(parent),
    ico_mini(30), state_width(6), titleTemplate(QStringLiteral("(%1) %2")) {

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
    icons.insert(Core::dt_site_vk,                                      PIXMAP(QStringLiteral(":/items/vk_item"), size));
    icons.insert(Core::dt_site_vk + SELECTION_ITER,                     PIXMAP(QStringLiteral(":/items/vk_item_on"), size));
    icons.insert(Core::dt_site_sc,                                      PIXMAP(QStringLiteral(":/items/sc_item"), size));
    icons.insert(Core::dt_site_sc + SELECTION_ITER,                     PIXMAP(QStringLiteral(":/items/sc_item_on"), size));
    icons.insert(Core::dt_site_od,                                      PIXMAP(QStringLiteral(":/items/od_item"), size));
    icons.insert(Core::dt_site_od + SELECTION_ITER,                     PIXMAP(QStringLiteral(":/items/od_item_on"), size));
    icons.insert(Core::dt_web,                                          PIXMAP(QStringLiteral(":/items/web_item"), size));
    icons.insert(Core::dt_web + SELECTION_ITER,                         PIXMAP(QStringLiteral(":/items/web_item_on"), size));
    icons.insert(Core::dt_local,                                        PIXMAP(QStringLiteral(":/items/local_item"), size));
    icons.insert(Core::dt_local + SELECTION_ITER,                       PIXMAP(QStringLiteral(":/items/local_item_on"), size));
    icons.insert(Core::dt_local_cue, icons[Core::dt_local]);
    icons.insert(Core::dt_local_cue + SELECTION_ITER, icons[Core::dt_local + SELECTION_ITER]);
    icons.insert(Core::dt_site_fourshared,                              PIXMAP(QStringLiteral(":/items/fourshared_item"), size));
    icons.insert(Core::dt_site_fourshared + SELECTION_ITER,             PIXMAP(QStringLiteral(":/items/fourshared_item_on"), size));
    icons.insert(Core::dt_site_zaycev,                                  PIXMAP(QStringLiteral(":/items/zaycev_item"), size));
    icons.insert(Core::dt_site_zaycev + SELECTION_ITER,                 PIXMAP(QStringLiteral(":/items/zaycev_item_on"), size));
    icons.insert(Core::dt_site_promodj,                                 PIXMAP(QStringLiteral(":/items/promodj_item"), size));
    icons.insert(Core::dt_site_promodj + SELECTION_ITER,                PIXMAP(QStringLiteral(":/items/promodj_item_on"), size));
    icons.insert(Core::dt_site_jetune,                                  PIXMAP(QStringLiteral(":/items/jetune_item"), size));
    icons.insert(Core::dt_site_jetune + SELECTION_ITER,                 PIXMAP(QStringLiteral(":/items/jetune_item_on"), size));
    icons.insert(Core::dt_site_redmp3,                                  PIXMAP(QStringLiteral(":/items/redmp3_item"), size));
    icons.insert(Core::dt_site_redmp3 + SELECTION_ITER,                 PIXMAP(QStringLiteral(":/items/redmp3_item_on"), size));
    icons.insert(Core::dt_site_yandex,                                  PIXMAP(QStringLiteral(":/items/yandex_item"), size));
    icons.insert(Core::dt_site_yandex + SELECTION_ITER,                 PIXMAP(QStringLiteral(":/items/yandex_item_on"), size));
    icons.insert(Core::dt_site_youtube,                                 PIXMAP(QStringLiteral(":/items/youtube_item"), size));
    icons.insert(Core::dt_site_youtube + SELECTION_ITER,                PIXMAP(QStringLiteral(":/items/youtube_item_on"), size));
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
    QVariant checkable = attrs.value(Keys::checkable);

    QString ext;
    int angle = bodyRect.height() / 2.2, ico_offset = 0, right_offset = bodyRect.right() - 12, top = option.rect.bottom(), left_offset = bodyRect.left() + 10;
    int background_state = attrs.value(Keys::state).toInt();
    bool is_folder = false, is_selected = option.state & (QStyle::State_Selected);

    QBrush fill_color;

    if (attrs.value(Keys::played).toBool())
        fill_color = Settings::obj().playedState(bodyRect, is_selected);
    else {
        switch (background_state) {
            case Core::ItemState::flag_new_item:
                fill_color = Settings::obj().defaultState(bodyRect, is_selected);
                break;
            case Core::ItemState::flag_listened:
                fill_color = Settings::obj().listenedState(bodyRect, is_selected);
                break;
            case Core::ItemState::flag_liked:
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
            ext = attrs.value(Keys::ext).toString();

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

        if (attrs[Keys::undefined].toBool())
            painter -> drawPixmap(rect, icons[-300]);
        else if (attrs[Keys::proccessing].toBool())
            painter -> drawPixmap(rect, icons[-200 + (is_selected ? SELECTION_ITER : 0)]);
        else {
            if (attrs[Keys::not_exist].toBool())
                painter -> drawPixmap(rect, icons[-100]);
            else if (attrs[Keys::unsupported].toBool())
                painter -> drawPixmap(rect, icons[-300]);
            else {
                if (Settings::obj().isShowSystemIcons()) {
                    QRect icoRect = QRect(
                                bodyRect.left() + 4 + (icon_size / 20),
                                option.rect.top() + (option.rect.height() - icon_size) / 2,
                                icon_size,
                                icon_size
                            );
                    QVariant iconVal = attrs.value(Keys::icon);

                    if (iconVal.isValid()) {
                        QIcon icon = qvariant_cast<QIcon>(iconVal);
                        painter -> drawPixmap(icoRect, icon.pixmap(QSize(icon_size, icon_size)));
                    }
                } else {
                    QRect pseudoIcoRect = QRect(
                                bodyRect.left() - 1,
                                option.rect.top() + 1,
                                icon_size + (ico_offset + 4),
                                bodyRect.height() + 1
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

                    if (attrs.value(Keys::shareable).toBool()) {
                        font.setPixelSize(extra_font_size);
                        painter -> setFont(font);

                        painter -> drawText(
                            pseudoIcoRect.topRight() + QPoint(-10, 13),
                            Keys::asterix
                        );
                    }
                }
            }
        }
    }

    if (!is_folder && Settings::obj().isShowInfo()) {
        painter -> setFont(itemInfoFont);

        if (attrs.contains(Keys::info)) {
            painter -> setPen(
                        is_selected ?
                            Settings::obj().selectedItemInfoTextColor()
                          :
                            Settings::obj().itemInfoTextColor()
                        );

            QStringList infos = attrs.value(Keys::info).toStringList();

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
        } else {
            bool is_error = attrs.contains(Keys::error);
            painter -> setPen(is_error ? Qt::red : QColor::fromRgb(0, 71, 207));

            int right_offset_with_corner = right_offset - (angle / 3);
            top = bodyRect.bottom() - fmfInfo -> height() - 2;

            QPoint bottomRight(right_offset_with_corner - 4, bodyRect.bottom());
            QRect rectText(QPoint(left_offset, top), bottomRight);
            QString s = fmfInfo -> elidedText(attrs.value(is_error ? Keys::error : Keys::warning).toString(), Qt::ElideRight, rectText.width());
            painter -> drawText(rectText, Qt::AlignLeft, s);
            painter -> drawLine(rectText.topLeft(), rectText.topRight());
        }
    }

    painter -> setFont(itemFont);
    painter -> setPen(textColor);

    QPoint topMLeft(left_offset, bodyRect.top() + 2);
    QPoint bottomMRight(right_offset, top - 2);

    QRect rectText2(topMLeft, bottomMRight);
    QString s = attrs.value(Keys::name).toString();
    if (Settings::obj().isShowNumber())
        s = titleTemplate.arg(QString::number(index.row() + 1), s);

    s = fmf -> elidedText(s, Qt::ElideRight, rectText2.width());
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
    QVariant checkable = attrs.value(Keys::checkable);

    int background_state = attrs.value(Keys::state).toInt();
    int angle = bodyRect.height() / 2, right_offset = bodyRect.right() - 12, top = option.rect.bottom(), left_offset = bodyRect.left() + 6;
    bool is_folder = false, is_selected = option.state & (QStyle::State_Selected);

    QBrush state_color;

    switch (background_state) {
        case Core::ItemState::flag_new_item:
            state_color = Settings::obj().defaultState(bodyRect, !is_selected);
            break;
        case Core::ItemState::flag_listened:
            state_color = Settings::obj().listenedState(bodyRect, !is_selected);
            break;
        case Core::ItemState::flag_liked:
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
        QRect icoRect = QRect(bodyRect.left() + (icon_size / 20), option.rect.top() + (option.rect.height() - icon_size) / 2, icon_size, icon_size);
        QRect rect(icoRect.left() + state_width, option.rect.top() + state_width * 1.5 + icon_size % 2, icon_size - state_width * 2, icon_size - state_width * 2);
        painter -> setBrush(Qt::NoBrush);

        if (attrs[Keys::undefined].toBool())
            painter -> drawPixmap(rect, icons[-300]);
        else if (attrs[Keys::proccessing].toBool()) {
            painter -> drawPixmap(rect, icons[-200 + (is_selected ? SELECTION_ITER : 0)]);
        } else {
            if (attrs[Keys::not_exist].toBool())
                painter -> drawPixmap(rect, icons[-100]);
            else if (attrs[Keys::unsupported].toBool())
                painter -> drawPixmap(rect, icons[-300]);
            else {
                if (Settings::obj().isShowSystemIcons()) {
                    QVariant iconVal = attrs.value(Keys::icon);
                    if (iconVal.isValid()) {
                        QIcon icon = qvariant_cast<QIcon>(iconVal);
                        painter -> drawPixmap(rect, icon.pixmap(QSize(icon_size, icon_size)));
                    }
                } else {
                    if (icon_size > 24) {
                        int selection = (is_selected ? SELECTION_ITER : 0);
                        painter -> drawPixmap(rect, icons.value(attrs[Keys::type].toInt() + selection, icons[dt_web + selection]));
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

        if (attrs.value(Keys::shareable).toBool()) {
            QFont font; font.setFamily(extra_font_name); font.setPixelSize(extra_font_size);
            painter -> setFont(font);
            painter -> setPen(Qt::NoPen);
            painter -> setBrush(QBrush(textColor));
            QPointF centerP = icoRect.topLeft() + QPointF(icoRect.width() / 2 + 0.5, 1);
            int rect_size = extra_font_size / 4;
            painter -> drawEllipse(centerP, rect_size, rect_size);
            painter -> setPen(is_selected ? Settings::obj().itemTextColor() : Settings::obj().selectedItemTextColor());
            painter -> drawText(centerP + QPointF(-rect_size, 9), Keys::asterix);
        }

        if (Settings::obj().isShowInfo()) {
            painter -> setFont(itemInfoFont);

            if (attrs.contains(Keys::info)) {
                painter -> setPen(
                            is_selected ?
                                Settings::obj().selectedItemInfoTextColor()
                              :
                                Settings::obj().itemInfoTextColor()
                            );

                QStringList infos = attrs.value(Keys::info).toStringList();

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
            } else {
                bool is_error = attrs.contains(Keys::error);

                painter -> setPen(is_error ? Qt::red : QColor::fromRgb(0, 71, 207));
                int right_offset_with_corner = right_offset - (angle / 3);
                top = bodyRect.bottom() - fmfInfo -> height() - 2;
                QPoint bottomRight(right_offset_with_corner, bodyRect.bottom());
                QRect rectText(QPoint(left_offset, top), bottomRight);
                QString s = fmfInfo -> elidedText(attrs.value(is_error ? Keys::error : Keys::warning).toString(), Qt::ElideRight, rectText.width());

                painter -> drawText(rectText, Qt::AlignLeft, s);
                painter -> drawLine(rectText.topLeft(), rectText.topRight());
            }
        }
    }

    painter -> setFont(itemFont);
    painter -> setPen(textColor);

    QPoint topMLeft(left_offset, bodyRect.top() + 2);
    QPoint bottomMRight(right_offset, top - 2);

    QRect rectText2(topMLeft, bottomMRight);
    QString s = attrs.value(Keys::name).toString();
    if (Settings::obj().isShowNumber())
        s = titleTemplate.arg(QString::number(index.row() + 1), s);
    s = fmf -> elidedText(s, Qt::ElideRight, rectText2.width());
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
