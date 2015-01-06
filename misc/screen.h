#ifndef SCREEN
#define SCREEN

#include <QApplication>
#include <QDesktopWidget>

class Screen {
public:
    static QRect screenRect(QWidget * target) {
        QDesktopWidget * w = QApplication::desktop();
        int screen_index = w -> screenNumber(target);
        if (screen_index != -1)
            return w -> availableGeometry(screen_index);
        else
            return QRect();
    }
};

#endif // SCREEN

