#ifndef SCREEN
#define SCREEN

#include <QApplication>
#include <QDesktopWidget>

class Screen {
public:
    static QList<QRect> screenRects() {
        QDesktopWidget * w = QApplication::desktop();
        QList<QRect> res;
        for(int loop1 = 0 ; loop1 < w -> screenCount(); loop1++)
            res << w -> availableGeometry(loop1);

        return res;
    }

    static QRect screenRect(QWidget * target) {
        QDesktopWidget * w = QApplication::desktop();
        int screen_index = w -> screenNumber(target);
        if (screen_index != -1)
            return w -> availableGeometry(screen_index);
        else
            return QRect();
    }

    static void screenSize(int & width, int & height) {
        QDesktopWidget * w = QApplication::desktop();
        width = w -> width();
        height = w -> height();
    }

    static void minimizeIfNeeded(int & width, int & height, float percentage) {
        int screen_width, screen_height;
        percentage /= 100;
        screenSize(screen_width, screen_height);

        width = qMin((int)(screen_width * percentage), width);
        height = qMin((int)(screen_height * percentage), height);
    }
};

#endif // SCREEN

