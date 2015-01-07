#include "toolbar.h"
#include <QLabel>
#include <QAction>

using namespace Playo3;

ToolBar::ToolBar(const QString &title, QWidget * parent) : QToolBar(title, parent) {
    setAcceptDrops(true);
    setObjectName("tool_" + title);
    setToolButtonStyle(Qt::ToolButtonFollowStyle);
    setStyleSheet("#" + objectName() + "{ background-color: rgba(212, 212, 212, 92); }");

    QLabel * titleLabel = new QLabel(this);
    titleLabel -> setStyleSheet("color: white;");
    QFont f = titleLabel -> font();
    f.setBold(true);
    f.setPixelSize(10);
    titleLabel -> setFont(f);
    QFontMetrics metrics(f);

    titleLabel -> setText(metrics.elidedText(title, Qt::ElideRight, 30, Qt::TextWrapAnywhere));
    titleLabel -> setMinimumSize(30, 30);
    titleLabel -> setMaximumSize(30, 30);
    titleLabel -> setAlignment(Qt::AlignCenter);
    titleLabel -> setContentsMargins(0,0,0,0);
    addWidget(titleLabel) -> setObjectName("*Title");
}

ToolBar::~ToolBar() {
}

void ToolBar::dragEnterEvent(QDragEnterEvent *event) {
   if (event -> mimeData() -> hasFormat("text/uri-list"))
       event -> accept();
   else
       event -> ignore();
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
        event -> accept();
    } else {  event -> ignore(); }
}
