#ifndef VIDEO_WIDGET_H
#define VIDEO_WIDGET_H

#include <qvideowidget.h>

class VideoWidget : public QVideoWidget {
    Q_OBJECT
public:
    VideoWidget(QWidget * parent = 0);

protected:
    void keyPressEvent(QKeyEvent * event);
    void mouseDoubleClickEvent(QMouseEvent * event);
};


#endif // VIDEO_WIDGET_H
