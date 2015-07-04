#include "dockbar.h"

#include <qdatetime.h>
#include <qdebug.h>

using namespace Playo3;

DockBar::DockBar(const QString & title, QWidget * parent, bool closable, Qt::WindowFlags flags)
    : QDockWidget(title, parent, flags), sticked(false), inProcess(false), mWidget(0), spinner(0) {

    installEventFilter(parent);

    setAttribute(Qt::WA_DeleteOnClose, closable);
    setObjectName(title + QString::number(QDateTime::currentMSecsSinceEpoch()));
    setTitleBarWidget((titleWidget = new WindowTitle(true, this, 26, QMargins(10, 0, 10, 0), QMargins(0, 8, 0, 0), 5, 0, false, false, false)));
    titleWidget -> addCustomButton("Rotate", QPixmap(":/controls/rotate_off"), QPixmap(":/controls/rotate_on"), this, SLOT(rotate()));
    titleWidget -> addMaxiButton(this, SLOT(toggleFloating()));
    titleWidget -> addCloseButton(this, SLOT(close()));
    setWindowTitle(title);

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    Stylesheets::initBrush(brush);

    connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(floatingChanged(bool)));
    connect(this, SIGNAL(dockLocationChanged(Qt::DockWidgetArea)), this, SLOT(onDockLocationChanged(Qt::DockWidgetArea)));
    useVerticalTitles(false);
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
        spinner = new Spinner("In process", 80, 80, this);
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
    Stylesheets::calcBorderRect(rect(), borderRect);
    QDockWidget::resizeEvent(event);

    brush.setStart(rect().topLeft());
    brush.setFinalStop(rect().bottomRight());
}

void DockBar::closeEvent(QCloseEvent * e) {
    emit closing();
    ((MainWindow *)parentWidget()) -> removeOuterChild(this);
    QDockWidget::closeEvent(e);
}

void DockBar::paintEvent(QPaintEvent * event) {
    QPainter painter(this);
    painter.save();

    if (isFloating()) {
        painter.setBrush(brush);
        painter.setPen(Stylesheets::pen);
        painter.drawRoundedRect(borderRect, Stylesheets::borderRadius, Stylesheets::borderRadius, Qt::AbsoluteSize);
        painter.setPen(Stylesheets::bevelPen);
        painter.drawRoundedRect(borderRect, Stylesheets::borderRadius, Stylesheets::borderRadius, Qt::AbsoluteSize);
    }
    else {
        QPen pen(Qt::lightGray);
        pen.setCosmetic(true);
        pen.setWidth(2);
        painter.setPen(pen);
        painter.drawRoundedRect(borderRect, Stylesheets::borderRadius, Stylesheets::borderRadius, Qt::AbsoluteSize);
    }

    painter.restore();
    event -> accept();
}

void DockBar::setTabBarSettings() {
    QList<QTabBar *> tabbars = parentWidget() -> findChildren<QTabBar *>(QString(), Qt::FindDirectChildrenOnly);
    QList<QTabBar *>::Iterator it = tabbars.begin();

    for(; it != tabbars.end(); it++) {
        (*it) -> setElideMode(Qt::ElideRight);
    }
}

TabifyParams DockBar::tabIndex() const {
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

    return TabifyParams();
}
