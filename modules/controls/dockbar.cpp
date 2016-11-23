#include "dockbar.h"
#include "video_widget_style.h"

#include <qdatetime.h>

using namespace Controls;

DockBar::DockBar(const QString & title, QWidget * parent, bool closable, Qt::WindowFlags flags, const QString & objName)
    : QDockWidget(title, parent, flags), sticked(false), inProcess(false), mWidget(0), spinner(0) {

    installEventFilter(parent);

    setObjectName(objName.isEmpty() ? title + QString::number(QDateTime::currentMSecsSinceEpoch()) : objName);
    setTitleBarWidget((titleWidget = new WindowTitle(true, this, 26, QMargins(10, 0, 10, 0), QMargins(0, 8, 0, 0), 5, 0, false, false, false)));
//  INFO: rotate is a little buggy
//    titleWidget -> addCustomButton(QStringLiteral("Rotate"), QPixmap(QStringLiteral(":/controls/rotate_off")), QPixmap(QStringLiteral(":/controls/rotate_on")), this, SLOT(rotate()));
    titleWidget -> addMaxiButton(this, SLOT(toggleFloating()));
    titleWidget -> addCloseButton(this, SLOT(close()));
    setWindowTitle(title);

    setAttribute(Qt::WA_TranslucentBackground, true);
    setAttribute(Qt::WA_DeleteOnClose, closable);

    connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChanged(bool)));
    connect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(onDockLocationChanged(Qt::DockWidgetArea)));
    useVerticalTitles(false);
    setStyle(new Controls::VideoWidegtStyle());
}

//bool DockBar::event(QEvent * event) {
//    qDebug() << event -> type();

//    switch(event -> type()) {
//        case QEvent::WindowActivate:
//        case QEvent::WindowDeactivate:
//            ;
//    }

//    return QDockWidget::event(event);
//}

void DockBar::markAsSticked() {
    sticked = true;
    if (parent())
        ((MainWindow *)parentWidget()) -> addOuterChild(this);
}

void DockBar::useVerticalTitles(bool vertical) {
    if (vertical) {
        setContentsMargins(0, 4, 3, 4);
        setFeatures(features() | QDockWidget::DockWidgetVerticalTitleBar);
    } else {
        setContentsMargins(3, 0, 3, 4);
        DockWidgetFeatures flags = features();
        flags &= ~QDockWidget::DockWidgetVerticalTitleBar;
        setFeatures(flags);
    }

    titleWidget -> setVertical(vertical);
}

void DockBar::onMoveInProcess() {
    inProcess = true;
    if (!spinner)
        spinner = new Spinner(QStringLiteral("In process"), 80, 80, this);
    else
        spinner -> setValue(SPINNER_IS_CONTINIOUS);

    mWidget = widget();
    setWidget(spinner);
    spinner -> show();
    mWidget -> hide();
}
void DockBar::onMoveOutProcess() {
    if (inProcess) {
        inProcess = false;
        setWidget(mWidget);
        spinner -> hide();
        spinner -> clear();
        mWidget -> show();
    }
}

void DockBar::onSetProgress(int percent) {
    if (spinner)
        spinner -> setValue(percent);
}

void DockBar::onSetProgress2(int percent) {
    if (spinner)
        spinner -> setValue2(percent);
}

void DockBar::resizeEvent(QResizeEvent * event) {
    Settings::currentStyle -> calcBorderRect(rect(), borderRect);
    QDockWidget::resizeEvent(event);
}

void DockBar::closeEvent(QCloseEvent * e) {
    emit closing();
    ((MainWindow *)parentWidget()) -> removeOuterChild(this);
    QDockWidget::closeEvent(e);
}

void DockBar::paintEvent(QPaintEvent * event) {
    switch(Settings::currentStyle -> styleType()) {
        case IStylesheets::light:
        case IStylesheets::dark: {
            QPainter painter(this);
            painter.save();

            Settings::currentStyle -> innerBrush.setStart(rect().topLeft());
            Settings::currentStyle -> innerBrush.setFinalStop(rect().topRight());
            painter.setBrush(Settings::currentStyle -> innerBrush);

            if (isFloating()) {
                painter.setPen(Settings::currentStyle -> pen);
                painter.drawRoundedRect(borderRect, Settings::currentStyle -> borderRadius, Settings::currentStyle -> borderRadius, Qt::AbsoluteSize);
                painter.setPen(Settings::currentStyle -> bevelPen);
            }
            else painter.setPen(Settings::currentStyle -> foregroundPen);

            painter.drawRoundedRect(borderRect, Settings::currentStyle -> borderRadius, Settings::currentStyle -> borderRadius, Qt::AbsoluteSize);

            painter.restore();
            event -> accept();
        break; }

        default: { QDockWidget::paintEvent(event); }
    }
}

void DockBar::setTabBarSettings() {
    QList<QTabBar *> tabbars = parentWidget() -> findChildren<QTabBar *>(QString(), Qt::FindDirectChildrenOnly);

    for(QList<QTabBar *>::Iterator it = tabbars.begin(); it != tabbars.end(); it++)
        (*it) -> setElideMode(Qt::ElideRight);
}

TabifyParams DockBar::tabIndex() const {
    if (titleWidget && parent()) {
        QWidget * widget = parentWidget();

        if (widget) {
            QList<QTabBar *> tabbars = widget -> findChildren<QTabBar *>(QString(), Qt::FindDirectChildrenOnly);
            QList<QTabBar *>::Iterator it = tabbars.begin();

            for(; it != tabbars.end(); it++) {
                for(int index = 0; index < (*it) -> count(); index++) {
                    if (this == ((DockBar *)((*it) -> tabData(index).toInt())))
                        return TabifyParams(*it, index);
                }
            }
        }
    }

    return TabifyParams();
}
