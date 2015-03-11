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

        InnerData * data;
        QList<InnerData *> l;

        QByteArray encoded = event -> mimeData() -> data(DROP_INNER_FORMAT);
        QDataStream stream(&encoded, QIODevice::ReadOnly);

        while (!stream.atEnd()) {
            data = new InnerData();
            stream >> data -> url >> data -> attrs;

            containPath = data -> attrs.contains(JSON_TYPE_PATH);

            if (requirePath) {
                if (!containPath)
                    data -> attrs.insert(JSON_TYPE_PATH, data -> url.toLocalFile().section('/', 0, -2));
            } else {
                if (containPath)
                    data -> attrs.remove(JSON_TYPE_PATH);
            }
        }
    } else if (data -> hasUrls()) {
        event -> accept();
        QList<QUrl>::Iterator it = event -> mimeData() -> urls().begin();

        for(; it != folderList.end(); it++) {
            QFileInfo file = QFileInfo((*it).toLocalFile());


        }
    }
    else event -> ignore();
}
