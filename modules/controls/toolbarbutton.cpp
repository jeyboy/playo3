#include "toolbarbutton.h"

using namespace Controls;
using namespace Views;

ToolbarButton::ToolbarButton(const QString & text, const QString & folderPath, QWidget * parent) : QToolButton(parent) {
    path = folderPath;

    if (!path.endsWith('/'))
        path = path.append('/') ;

    QFile::setPermissions(path, QFile::WriteUser | QFile::ReadUser);

    setProperty("custom", true);
    setText(text);
    setAcceptDrops(true);
    checkState();
    /*setDefaultDropAction(Qt::MoveAction);*/
}

void ToolbarButton::checkState() { Settings::currentStyle -> applyProperty(this, "error", !QFile::exists(path)); }

void ToolbarButton::dragEnterEvent(QDragEnterEvent * event) {
    if (event -> mimeData() -> hasFormat(QStringLiteral("text/uri-list")))
        event -> accept();
    else event -> ignore();
}

void ToolbarButton::dropEvent(QDropEvent * event) {
    DownloadView::obj().proceedDrop(event, path);

    if (event -> mimeData() -> hasFormat(DROP_INNER_FORMAT))
        ((IView *) event -> source()) -> markSelectedAsLiked(true);
}
