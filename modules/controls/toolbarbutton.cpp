#include "toolbarbutton.h"
#include "misc/stylesheets.h"

using namespace Playo3;

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

void ToolbarButton::dragEnterEvent(QDragEnterEvent * event) {
    if (event -> mimeData() -> hasFormat("text/uri-list"))
        event -> accept();
    else event -> ignore();
}

void ToolbarButton::dropEvent(QDropEvent * event) {
    DownloadView * dwn = DownloadView::instance();

    if (event -> mimeData() -> hasFormat(DROP_INNER_FORMAT)) {
        event -> accept();

        QByteArray encoded = event -> mimeData() -> data(DROP_INNER_FORMAT);
        QDataStream stream(&encoded, QIODevice::ReadOnly);

        while (!stream.atEnd()) {
            InnerData data;
            stream >> data.url >> data.attrs;

            dwn -> addRow(data.url, path, downloadTitle(data.attrs[JSON_TYPE_TITLE].toString(), data.attrs[JSON_TYPE_EXTENSION].toString()));
        }
    } else if (event -> mimeData() -> hasUrls()) {
        event -> accept();
        QList<QUrl>::Iterator it = event -> mimeData() -> urls().begin();

        for(; it != event -> mimeData() -> urls().end(); it++) {
            QFileInfo file = QFileInfo((*it).toLocalFile());
            dwn -> addRow((*it), path, downloadTitle(file.baseName(), file.completeSuffix()));
        }
    }
    else event -> ignore();
}
