#include "video_widget.h"
#include "player/player_index.h"

#include <qevent.h>

VideoWidget::VideoWidget(QWidget * parent) : QVideoWidget(parent) {
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::black);
    setPalette(p);

    setAttribute(Qt::WA_OpaquePaintEvent);

    panel = new Controls::ControlPanel(this);

    PlayerFactory::obj().registerCallback(call_to_player, this, SIGNAL(clicked()), SLOT(playPause()));
//
    setMinimumSize(VIDEO_DEFAULT_SIZE, VIDEO_DEFAULT_SIZE);
    show();
}

void VideoWidget::showEvent(QShowEvent * event) {
    panel -> setRegion(rect());
    QVideoWidget::showEvent(event);
}

void VideoWidget::keyPressEvent(QKeyEvent * event) {
    if (event -> key() == Qt::Key_Escape && isFullScreen()) {
        setFullScreen(false);
        setMinimumSize(VIDEO_DEFAULT_SIZE, VIDEO_DEFAULT_SIZE);
        show();
        event -> accept();
    } else if (event -> key() == Qt::Key_Enter && event -> modifiers() & Qt::Key_Alt) {
        toggleFullScreen();
        event -> accept();
    }
    else QVideoWidget::keyPressEvent(event);
}

void VideoWidget::mousePressEvent(QMouseEvent * event) {
    if (event -> buttons() & Qt::LeftButton)
        emit clicked();

    QVideoWidget::mousePressEvent(event);
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent * event) {
    toggleFullScreen();
    event -> accept();
}

void VideoWidget::resizeEvent(QResizeEvent * event) {
    panel -> setRegion(rect());
    QVideoWidget::resizeEvent(event);
}

void VideoWidget::toggleFullScreen() {
    bool in_full = isFullScreen();

    if (!in_full) { setMinimumSize(width(), height()); }

    setFullScreen(!in_full);

    if (in_full) {
        setMinimumSize(VIDEO_DEFAULT_SIZE, VIDEO_DEFAULT_SIZE);
        show();
    }
}
