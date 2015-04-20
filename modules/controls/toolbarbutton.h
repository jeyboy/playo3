#ifndef TOOLBARBUTTON_H
#define TOOLBARBUTTON_H

#include <QToolButton>
#include <QMimeData>
#include <QMouseEvent>
#include <QFile>
#include <QMessageBox>
#include <qfileinfo.h>

#include "modules/data_struct/downloads/download_view.h"

namespace Playo3 {
    class ToolbarButton : public QToolButton {
        Q_OBJECT

    public:
        ToolbarButton(QString text, QString folderPath, QWidget * parent = 0);
        inline QString mainPath() const { return path; }
        void checkState();
    protected:
        void dropEvent(QDropEvent * event);
        void dragEnterEvent(QDragEnterEvent * event);

    private:
        QString path;
    };
}

#endif // TOOLBARBUTTON_H
