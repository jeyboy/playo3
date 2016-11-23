#ifndef VIDEO_VIEW_H
#define VIDEO_VIEW_H

#include <qgraphicsvideoitem.h>
#include <qgraphicsview.h>
#include <qevent.h>
//#include <qpainter.h>

#define VIDEO_DEFAULT_SIZE 100

class VideoView : public QGraphicsView {
    Q_OBJECT

    QGraphicsVideoItem * item;
public:
    VideoView(QWidget * parent = 0);

    QGraphicsVideoItem * videoItem() { return item; }
protected:
    void keyPressEvent(QKeyEvent * event);
    void mouseDoubleClickEvent(QMouseEvent * event);
    void resizeEvent(QResizeEvent * event);
//    void paintEvent(QPaintEvent * event) {
//        QPainter painter(this);
//        painter.setCompositionMode(QPainter::RasterOp_SourceAndDestination);
//        QVideoWidget::paintEvent(event);
//    }

    void toggleFullScreen();
};

#endif // VIDEO_VIEW_H
