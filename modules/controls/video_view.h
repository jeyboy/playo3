#ifndef VIDEO_VIEW_H
#define VIDEO_VIEW_H

#include <qgraphicsvideoitem.h>
#include <qgraphicsview.h>
#include <qevent.h>
#include <QStyleOptionGraphicsItem>
//#include <qpainter.h>

#define VIDEO_DEFAULT_SIZE 100

class GraphicsVideoItem : public QGraphicsVideoItem {
protected:
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = Q_NULLPTR) {
        painter -> setCompositionMode(QPainter::RasterOp_SourceXorDestination);
        QGraphicsVideoItem::paint(painter, option, widget);
    }
};

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
    void toggleFullScreen();
};

#endif // VIDEO_VIEW_H
