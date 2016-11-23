#include "video_view.h"

#include <qgraphicsscene.h>

VideoView::VideoView(QWidget * parent) : QGraphicsView(parent) {
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    setAttribute(Qt::WA_OpaquePaintEvent);

    setMinimumSize(VIDEO_DEFAULT_SIZE, VIDEO_DEFAULT_SIZE);

    QGraphicsScene * scene = new QGraphicsScene(this);
    item = new QGraphicsVideoItem();
    item -> setOpacity(1);

    setScene(scene);
    scene -> addItem(item);

    scene -> setBackgroundBrush(Qt::black);

    fitInView(item, Qt::KeepAspectRatio);
}

void VideoView::resizeEvent(QResizeEvent * e) {
//    QRectF bounds = scene->itemsBoundingRect();
//    bounds.setWidth(bounds.width()*0.9);         // to tighten-up margins
//    bounds.setHeight(bounds.height()*0.9);       // same as above
//    ui->graphicsView->fitInView(bounds, Qt::KeepAspectRatio);
//    ui->graphicsView->centerOn(0, 0);

    fitInView(item, Qt::KeepAspectRatio);
    QGraphicsView::resizeEvent(e);
}

void VideoView::keyPressEvent(QKeyEvent * event) {
    /*if (event -> key() == Qt::Key_Escape && isFullScreen()) {
        setFullScreen(false);
        setMinimumSize(VIDEO_DEFAULT_SIZE, VIDEO_DEFAULT_SIZE);
        event -> accept();
    } else if (event -> key() == Qt::Key_Enter && event -> modifiers() & Qt::Key_Alt) {
        toggleFullScreen();
        event -> accept();
    }
    else*/ QGraphicsView::keyPressEvent(event);
}

void VideoView::mouseDoubleClickEvent(QMouseEvent * event) {
    toggleFullScreen();
    event -> accept();
}

void VideoView::toggleFullScreen() {
//    bool in_full = isFullScreen();

//    if (!in_full) { setMinimumSize(width(), height()); }

//    setFullScreen(!in_full);

//    if (in_full) { setMinimumSize(VIDEO_DEFAULT_SIZE, VIDEO_DEFAULT_SIZE); }
}
