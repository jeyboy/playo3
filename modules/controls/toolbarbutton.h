#ifndef TOOLBARBUTTON_H
#define TOOLBARBUTTON_H

#include <QToolButton>
#include <QMimeData>
#include <QMouseEvent>
#include <QFile>
#include <QMessageBox>

//#include "model/view.h"
//#include "web/socials/vk_api.h"

class ToolbarButton : public QToolButton {
    Q_OBJECT

protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);

public:
    ToolbarButton(QString text, QString folderPath, QWidget * parent=0);

    QString path;
};

#endif // TOOLBARBUTTON_H
