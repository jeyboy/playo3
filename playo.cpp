#include "playo.h"
#include "ui_playo.h"
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>
#include <qbitmap.h>

Playo::Playo(QWidget * parent) : QMainWindow(parent),
    ui(new Ui::Playo), borderWidth(4), radius(12),
    resizeFlagX(false), resizeFlagY(false),
    moveFlag(false), brush(0) {
    ui -> setupUi(this);

    QApplication::setWindowIcon(QIcon(":ico"));
    setWindowTitle("Playo");
    setAcceptDrops(true);
    setContentsMargins(borderWidth * 2, borderWidth * 2, borderWidth * 2, borderWidth * 2);

    setWindowFlags(Qt::Widget | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);
//    setAttribute(Qt::WA_PaintOnScreen);

//    setAttribute(Qt::WA_DeleteOnClose);

    initMenuWidget();
    init();
}

Playo::~Playo() {
    delete ui;
}

//stop(0%,rgba(174,188,191,1)), color-stop(50%,rgba(110,119,116,1)), color-stop(51%,rgba(10,14,10,1)), color-stop(100%,rgba(10,8,9,1))); /*

void Playo::init() {
    QLinearGradient grad(0, 0, rect().width(), 0);
    grad.setColorAt(0, QColor::fromRgb(181,189,200));
    grad.setColorAt(0.5, QColor::fromRgb(40,52,59));
    grad.setColorAt(1, QColor::fromRgb(130,140,149));

    pen.setBrush(grad);
    pen.setWidth(borderWidth);
    pen.setCosmetic(true);

//    stop(0%,rgba(255,255,255,1)), color-stop(50%,rgba(241,241,241,1)), color-stop(51%,rgba(225,225,225,1)), color-stop(100%,rgba(246,246,246,1))); /*

    addToolBar("lalka");
    addToolBar("lalka2");
}

void Playo::initMenuWidget() {
    QLabel *coolLabel = new QLabel("The coolest text in the world", this);
    QWidget *menuWidget = new QWidget;
    QGridLayout *vLayout = new QGridLayout();
    menuWidget -> setLayout(vLayout);
    vLayout -> addWidget(coolLabel, 0, 0, Qt::AlignLeft);
    QPushButton * close = new QPushButton("Close", this);
    connect(close, SIGNAL(clicked()), this, SLOT(close()));
    vLayout -> addWidget(close, 0, 1, Qt::AlignRight);
    setMenuWidget(menuWidget);
}

void Playo::resizeEvent(QResizeEvent * event) {
    delete brush;
    brush = new QLinearGradient(0, 0, rect().width(), rect().height());
    brush -> setColorAt(0, QColor::fromRgb(181, 189, 200, 224));
    brush -> setColorAt(.36, QColor::fromRgb(130, 140, 149, 224));
    brush -> setColorAt(1, QColor::fromRgb(40, 52, 59, 224));


//    brush -> setColorAt(0, QColor::fromRgb(207,231,250));
////    brush -> setColorAt(0.5, QColor::fromRgb(241,241,241));
////    brush -> setColorAt(0.51, QColor::fromRgb(225,225,225));
//    brush -> setColorAt(1, QColor::fromRgb(99,147,193));

    QMainWindow::resizeEvent(event);
}

void Playo::mousePressEvent(QMouseEvent * event) {
    if (event -> button() == Qt::LeftButton) {
        moveFlag = !isResizeable();
        dragPos = event -> globalPos();
        geom = geometry();
    }

    QMainWindow::mousePressEvent(event);
}

void Playo::mouseMoveEvent(QMouseEvent * event) {
    if (event -> buttons() & Qt::LeftButton) {
        if (resizeFlagX || resizeFlagY) {
            QSize ns;
            if (atBottom)
                ns.rheight() = geom.height() + (event -> globalPos().y() - dragPos.y());
            else
                ns.rheight() = geom.height() - (event -> globalPos().y() - dragPos.y());

            if (atLeft)
                ns.rwidth() = geom.width() - (event -> globalPos().x() - dragPos.x());
            else
                ns.rwidth() = geom.width() + (event -> globalPos().x() - dragPos.x());

            ns = QLayout::closestAcceptableSize(this, ns);

            QPoint p;
            QRect nr(p, ns);
            if (atBottom) {
                if (atLeft)
                    nr.moveTopRight(geom.topRight());
                else
                    nr.moveTopLeft(geom.topLeft());
            } else {
                if (atLeft)
                    nr.moveBottomRight(geom.bottomRight());
                else
                    nr.moveBottomLeft(geom.bottomLeft());
            }
            setGeometry(nr);
            QMainWindow::mouseMoveEvent(event);
        }
        else if(moveFlag)
            move(event -> globalPos() - (dragPos - geom.topLeft()));
        else
            QMainWindow::mouseMoveEvent(event);
    } else QMainWindow::mouseMoveEvent(event);
}

void Playo::paintEvent(QPaintEvent *) {
    QPainter painter(this);
//    painter.save();
    painter.setPen(pen);
    painter.setBrush(*brush);
    int offset = borderWidth;
    QRect dRect(rect().x() + offset, rect().y() + offset, rect().width() - offset * 2, rect().height() - offset * 2);
    painter.drawRoundedRect(dRect, radius, radius, Qt::AbsoluteSize);
    painter.drawPixmap(QRect(rect().width()/2 - rect().width()/4, rect().height()/2 - rect().height()/4, rect().width()/2, rect().height()/2), QPixmap(":main"));
}

bool Playo::isResizeable() {
    QPoint pos = mapFromGlobal(QCursor::pos());

    resizeFlagX = ((atLeft = (pos.x() >= 0 && pos.x() <= borderWidth * 3)) || (pos.x() >= width() - borderWidth * 3 && pos.x() <= width()));
    resizeFlagY = ((atBottom = (pos.y() >= height() - borderWidth * 3 && pos.y() <= height())) || (pos.y() >= 0 && pos.y() <= borderWidth * 3));

    return resizeFlagX || resizeFlagY;
}
