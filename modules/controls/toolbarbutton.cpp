#include "toolbarbutton.h"
#include "stylesheets.h"

using namespace Controls;
using namespace View;

ToolbarButton::ToolbarButton(QString text, QString folderPath, QWidget * parent) : QToolButton(parent) {
    path = folderPath;

    if (!path.endsWith('/'))
        path = path.append('/') ;

    QFile::setPermissions(path, QFile::WriteUser | QFile::ReadUser);

    setText(text);
    setStyleSheet(Stylesheets::toolbarButtonStyle());
    setAcceptDrops(true);
    checkState();
    /*setDefaultDropAction(Qt::MoveAction);*/
}

void ToolbarButton::checkState() {
    setProperty("error", !QFile::exists(path));
    style() -> unpolish(this);
    style() -> polish(this);
    update();
}

void ToolbarButton::dragEnterEvent(QDragEnterEvent * event) {
    if (event -> mimeData() -> hasFormat(QStringLiteral("text/uri-list")))
        event -> accept();
    else event -> ignore();
}

void ToolbarButton::dropEvent(QDropEvent * event) {
    DownloadView * dwn = DownloadView::instance();
    dwn -> proceedDrop(event, path);

    if (event -> mimeData() -> hasFormat(DROP_INNER_FORMAT))
        ((IView *) event -> source()) -> markSelectedAsLiked(true);
}
