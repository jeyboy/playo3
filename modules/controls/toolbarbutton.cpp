#include "toolbarbutton.h"
#include "misc/stylesheets.h"
#include <QDebug>

ToolbarButton::ToolbarButton(QString text, QString folderPath, QWidget * parent) : QToolButton(parent) {
    path = folderPath;

    if (!path.endsWith('/'))
        path = path.append('/') ;

    QFile::setPermissions(path, QFile::WriteUser | QFile::ReadUser);

    setText(text);
    setStyleSheet(Stylesheets::toolbarButtonStyle());
    setAcceptDrops(true);
    /*setDefaultDropAction(Qt::MoveAction);*/
}

void ToolbarButton::dragEnterEvent(QDragEnterEvent *event) {
    if (event -> mimeData() -> hasFormat("text/uri-list")) {
        event -> accept();
    } else {
        event -> ignore();
    }
}

void ToolbarButton::dropEvent(QDropEvent *event) {
    if (event -> mimeData() -> hasUrls()) {
        event -> accept();
//        View * view = (View *)event -> source();

//        if (!QString(view -> metaObject() -> className()).endsWith("View"))
//            qDebug() << "Out request";

//        view -> downloadSelected(path, true);
    } else { event -> ignore(); }
}
