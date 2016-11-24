#ifndef VIDEO_WIDGET_H
#define VIDEO_WIDGET_H

#include <qvideowidget.h>
#include <qpainter.h>

#define VIDEO_DEFAULT_SIZE 100

class VideoWidget : public QVideoWidget {
    Q_OBJECT
public:
    VideoWidget(QWidget * parent = 0);
protected:
    void keyPressEvent(QKeyEvent * event);
    void mouseDoubleClickEvent(QMouseEvent * event);
//    void paintEvent(QPaintEvent * event) {
//        QPainter painter(this);
//        painter.setCompositionMode(QPainter::RasterOp_SourceXorDestination);
//        QVideoWidget::paintEvent(event);
//    }

    void toggleFullScreen();
};


#endif // VIDEO_WIDGET_H
