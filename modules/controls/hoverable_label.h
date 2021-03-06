#ifndef HOVERABLE_LABEL_H
#define HOVERABLE_LABEL_H

#include "clickable_label.h"

namespace Controls {
    class HoverableLabel : public ClickableLabel {
      Q_OBJECT
    public:
        explicit HoverableLabel(QString user_text, const QPixmap & icon, const QPixmap & hoverIcon, QWidget * parent = 0, Qt::WindowFlags f = 0, const QObject * receiver = 0, const char * slot = 0);
        ~HoverableLabel();

        inline void setOn() { hoverIn(); blockEvent = true; }
        inline void setOff() { hoverOut(); blockEvent = false; }
    protected slots:
        virtual inline void hoverIn() { if (!blockEvent) setPixmap(hico); }
        virtual inline void hoverOut() { if (!blockEvent) setPixmap(ico); }
    private:
        bool blockEvent;
        QPixmap ico;
        QPixmap hico;
    };
}

#endif // HOVERABLE_LABEL_H
