#ifndef TOOLBARBUTTON_H
#define TOOLBARBUTTON_H

#include <qtoolbutton.h>
#include <qmimedata.h>
#include <qevent.h>
#include <qfileinfo.h>

#include "modules/data_struct/downloads/download_view.h"
#include "modules/data_struct/view_interface.h"

namespace Playo3 {
    class ToolbarButton : public QToolButton {
        Q_OBJECT
    public:
        ToolbarButton(QString text, QString folderPath, QWidget * parent = 0);
        inline QString mainPath() const { return path; }
        void checkState();
        inline Qt::KeyboardModifiers keyboardModifiers() const { return keyModifiers; }
    protected:
        inline void enterEvent(QEvent *) { checkState(); }
        inline void mousePressEvent(QMouseEvent * event) {
            checkState();
            keyModifiers = event -> modifiers();
            QToolButton::mousePressEvent(event);
        }
        void dropEvent(QDropEvent * event);
        void dragEnterEvent(QDragEnterEvent * event);

    private:
        QString path;
        Qt::KeyboardModifiers keyModifiers;
    };
}

#endif // TOOLBARBUTTON_H
