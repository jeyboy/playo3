#ifndef TOOLBAR_DROP_BUTTON_H
#define TOOLBAR_DROP_BUTTON_H

#include <qtoolbutton.h>
#include <qmimedata.h>
#include <qevent.h>

namespace Controls {
    class ToolbarDropButton : public QToolButton {
        Q_OBJECT
    public:
        ToolbarDropButton(const QString & text, QWidget * parent = 0);
        inline Qt::KeyboardModifiers keyboardModifiers() const { return keyModifiers; }
    protected:
        virtual void dropReaction(QDropEvent * event) = 0;

        inline void mousePressEvent(QMouseEvent * event) {
            keyModifiers = event -> modifiers();
            QToolButton::mousePressEvent(event);
        }
        void dropEvent(QDropEvent * event);
        void dragEnterEvent(QDragEnterEvent * event);

    private:
        Qt::KeyboardModifiers keyModifiers;
    };
}

#endif // TOOLBAR_DROP_BUTTON_H
