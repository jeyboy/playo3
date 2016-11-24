#include "video_widget.h"
//#include "video_widget_style.h"

#include <qevent.h>

VideoWidget::VideoWidget(QWidget * parent) : QVideoWidget(parent) {
    setWindowFlags(Qt::Window);
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::black);
    setPalette(p);

    setAttribute(Qt::WA_OpaquePaintEvent);

    setMinimumSize(VIDEO_DEFAULT_SIZE, VIDEO_DEFAULT_SIZE);
    show();
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

void VideoWidget::mouseDoubleClickEvent(QMouseEvent * event) {
    toggleFullScreen();
    event -> accept();
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
