#include "toolbar.h"
#include <QDebug>

ToolBar::ToolBar(const QString &title, QWidget * parent) : QToolBar(title, parent) {
    setAcceptDrops(true);
    setObjectName("tool_" + title);
    setToolButtonStyle(Qt::ToolButtonFollowStyle);
}

ToolBar::~ToolBar() {
}

void ToolBar::dragEnterEvent(QDragEnterEvent *event) {
   if (event -> mimeData() -> hasFormat("text/uri-list")) {
       event -> accept();
   } else {
       event -> ignore();
   }
}

void ToolBar::dropEvent(QDropEvent *event) {
    if(event -> mimeData() -> hasUrls()) {
        QList<QUrl> list = event -> mimeData() -> urls();
        foreach(QUrl url, list) {
            if (url.isLocalFile()) {
                QFileInfo file = QFileInfo(url.toLocalFile());
                if (file.isDir()) {
                    emit folderDropped((file.baseName().isEmpty() ? file.filePath() : file.baseName()), file.filePath());
                }
            }
        }
        event->accept();
    } else {  event -> ignore(); }
}
