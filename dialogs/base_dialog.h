#ifndef BASE_DIALOG
#define BASE_DIALOG

#include <qdialog.h>
#include <qpainter.h>

#include "settings.h"

class BaseDialog : public QDialog {
protected:
    void paintEvent(QPaintEvent * event) {
        switch(Settings::currentStyle -> styleType()) {
            case IStylesheets::light:
            case IStylesheets::dark: {
                QPainter painter(this);
                painter.save();

                Settings::currentStyle -> mainBrush.setStart(rect().topLeft());
                Settings::currentStyle -> mainBrush.setFinalStop(rect().bottomRight());
                painter.setBrush(Settings::currentStyle -> mainBrush);


                Settings::currentStyle -> initMainBrush(rect());
                painter.setBrush(Settings::currentStyle -> mainBrush);
                painter.setPen(Qt::NoPen);
                painter.drawRect(rect());
                painter.restore();
            break; }

            default: { QDialog::paintEvent(event); }
        }
    }
public:
    BaseDialog(QWidget * parent = 0, Qt::WindowFlags f = 0) : QDialog(parent, f) {
        setAttribute(Qt::WA_TranslucentBackground, true);
    }
};

#endif // BASE_DIALOG
