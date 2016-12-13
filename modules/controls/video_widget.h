#ifndef VIDEO_WIDGET_H
#define VIDEO_WIDGET_H

#include <qvideowidget.h>
#include <qpainter.h>

#include "control_panel.h"

#define VIDEO_DEFAULT_SIZE 100

class VideoWidget : public QVideoWidget {
    Q_OBJECT

    Controls::ControlPanel * panel;
public:
    VideoWidget(QWidget * parent = 0);

signals:
    void clicked();

protected:
    void showEvent(QShowEvent * event);
    void keyPressEvent(QKeyEvent * event);
    void mouseDoubleClickEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);
    void resizeEvent(QResizeEvent * event);
    void toggleFullScreen();
};


#endif // VIDEO_WIDGET_H
