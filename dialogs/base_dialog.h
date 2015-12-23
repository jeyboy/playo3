#ifndef BASE_DIALOG
#define BASE_DIALOG

#include <qdialog.h>
#include <qpainter.h>

#include "settings.h"

class BaseDialog : public QDialog {
protected:
    void paintEvent(QPaintEvent * event) {
        QPainter painter(this);

        switch(Settings::currentStyle -> styleType()) {
            case IStylesheets::light:
                painter.setPen(Qt::NoPen);
                painter.setBrush(QColor(224, 224, 224, 224));
                painter.drawRoundedRect(rect(), Settings::currentStyle -> borderRadius, Settings::currentStyle -> borderRadius, Qt::AbsoluteSize);

            case IStylesheets::dark: {
                painter.save();

                Settings::currentStyle -> mainBrush.setStart(rect().topLeft());
                Settings::currentStyle -> mainBrush.setFinalStop(rect().topRight());
                painter.setBrush(Settings::currentStyle -> mainBrush);
                painter.drawRoundedRect(rect(), Settings::currentStyle -> borderRadius, Settings::currentStyle -> borderRadius, Qt::AbsoluteSize);

                painter.restore();
            break; }

            default: { QDialog::paintEvent(event); }
        }
    }
public:
    BaseDialog(QWidget * parent = 0, Qt::WindowFlags f = 0) : QDialog(parent, f | Qt::FramelessWindowHint) {
        setAttribute(Qt::WA_TranslucentBackground, true);
    }
};

#endif // BASE_DIALOG
