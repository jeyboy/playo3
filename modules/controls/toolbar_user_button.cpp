#include "toolbar_user_button.h"

using namespace Controls;
using namespace Views;

ToolbarUserButton::ToolbarUserButton(const QString & text, const QString & folderPath, QWidget * parent) : ToolbarDropButton(text, parent) {
    path = folderPath;

    if (!path.endsWith('/'))
        path = path.append('/') ;

    QFile::setPermissions(path, QFile::WriteUser | QFile::ReadUser);

    setProperty("custom", true);
    checkState();
}

void ToolbarUserButton::checkState() { Settings::currentStyle -> applyProperty(this, "error", !QFile::exists(path)); }

void ToolbarUserButton::dropReaction(QDropEvent * event) {
    DownloadView::obj().proceedDrop(event, path);

    if (event -> mimeData() -> hasFormat(DROP_INNER_FORMAT))
        ((IView *) event -> source()) -> markSelectedAsLiked(true);
}
