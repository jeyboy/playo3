#ifndef BUTTON
#define BUTTON

#include <qpushbutton.h>

namespace Playo3 {
    class Button : public QPushButton {
        Q_OBJECT
    public:
        Button(const QString & text, const QObject * receiver, const char *slot, QWidget * parent = 0);
        Button(const QIcon& icon, const QObject * receiver, const char *slot, QWidget * parent = 0);
        Button(const QIcon& icon, const QString &text, const QObject * receiver, const char *slot, QWidget * parent = 0);
    private:
        void setStyle();
    };
}

#endif // BUTTON
