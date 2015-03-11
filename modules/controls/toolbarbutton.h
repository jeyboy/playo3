#ifndef TOOLBARBUTTON_H
#define TOOLBARBUTTON_H

#include <QToolButton>
#include <QMimeData>
#include <QMouseEvent>
#include <QFile>
#include <QMessageBox>
#include <qfileinfo.h>

#include "misc/file_utils/filename_conversions.h"

#include "modules/data_struct/item_drop_formats.h"
#include "modules/data_struct/downloads/download_view.h"

//#include "model/view.h"
//#include "web/socials/vk_api.h"

namespace Playo3 {
    class ToolbarButton : public QToolButton {
        Q_OBJECT

    public:
        ToolbarButton(QString text, QString folderPath, QWidget * parent = 0);
        QString mainPath() const { return path; }

    protected:
        void dropEvent(QDropEvent * event);
        void dragEnterEvent(QDragEnterEvent * event);

    private:
        QString path;
    };
}

#endif // TOOLBARBUTTON_H
