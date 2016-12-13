#include "control_panel.h"

#include <qpainter.h>
#include <qstyleoption.h>

using namespace Controls;

ControlPanel::ControlPanel(QWidget * parent) : QFrame(parent) {
//    setAttribute(Qt::WA_OpaquePaintEvent);
//    setAttribute(Qt::WA_PaintOnScreen);
//    setAttribute(Qt::WA_NoSystemBackground);
//    setAttribute(Qt::WA_LayoutUsesWidgetRect);

    setWindowFlags(Qt::WindowStaysOnTopHint);

//    setBackgroundRole(QPalette::Shadow);
//    setAutoFillBackground(true);
    setMaximumHeight(60);
    setMinimumHeight(60);

    setStyleSheet(
                "ControlPanel {"
                "   background: rgba(44, 44, 44, 72);"
                "   border-radius: 16px;"
                "}"
                "QPushButton {"
                "   border-radius: 16px;"
                "   border: 2px solid #000;"
                "   background-color: #ddd;"
                "}"
                "QPushButton:hover {"
                "   background-color: #bbb;"
                "}"
//                "QPushButton:pressed {"
//                "   background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #dadbde, stop: 1 #f6f7fa);"
//                "}"
//                QPushButton:on {
//                        background: qlineargradient(x1 : 0, y1 : 0, x2 : 0, y2 :   1, stop :   0.0 #5AA72D,
//                        stop :   0.5 #B3E296, stop :   0.55 #B3E296, stop :   1.0 #f5f9ff);
//                        padding-top: 2px;
//                        padding-left: 3px;
//                }

//                QPushButton:disabled {
//                        background: transparent #e5e9ee;
//                        padding-top: 2px;
//                        padding-left: 3px;
//                        color: black;
//                }
                "QLabel {"
                "   color: #fff;"
                "   font-weight: bold;"
                "}"
    );

    QHBoxLayout * panelLayout = new QHBoxLayout(this);
    setLayout(panelLayout);
    setVisible(false); // remove later

    play = new QPushButton(QIcon(":play"), "", this);
    play -> setMaximumSize(40, 50);
    play -> setMinimumWidth(40);
//    connect(play, SIGNAL(clicked()), (MediaPlayer *)clock -> mediaPlayer(), SLOT(play()));
    layout() -> addWidget(play);

    pause = new QPushButton(QIcon(":pause"), "", this);
    pause -> setMaximumSize(40, 50);
    pause -> setMinimumWidth(40);
//    connect(pause, SIGNAL(clicked()), (MediaPlayer *)clock -> mediaPlayer(), SLOT(pause()));
    layout() -> addWidget(pause);

    stop = new QPushButton(QIcon(":stop"), "", this);
    stop -> setMaximumSize(40, 50);
    stop -> setMinimumWidth(40);
//    connect(stop, SIGNAL(clicked()), (MediaPlayer *)clock -> mediaPlayer(), SLOT(stop()));
    layout() -> addWidget(stop);

    timer = new QLabel(this);
    layout() -> addWidget(timer);

    slider = new ClickableSlider(-1, this);
    slider -> setStyle(new SliderStyle());
    slider -> setTickInterval(60000);
    slider -> setOrientation(Qt::Horizontal);
    slider -> setMinimumSize(100, 30);

//    slider -> setMaximum(((MediaPlayer *) clock -> mediaPlayer()) -> durationMillis());

    layout() -> addWidget(slider);
}

QRect ControlPanel::getRegion() const { return region; }

void ControlPanel::setRegion(const QRect & rect) {
    region = rect;

//    region.setBottom(rect.top() + minimumHeight() + 10);
    region.setTop(region.bottom() - (minimumHeight() + 10));
    region.setLeft(10); region.setRight(region.right() - 10);
    setGeometry(region);
}

void ControlPanel::paintEvent(QPaintEvent *) {
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style() -> drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
