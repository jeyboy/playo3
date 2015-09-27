#include "toolbar.h"
#include <qlabel.h>
#include <qaction.h>

using namespace Controls;

ToolBar::ToolBar(const QString & title, QWidget * parent) : QToolBar(title, parent) {
    setAcceptDrops(true);
    setObjectName(QStringLiteral("tool_") % title);
    setToolButtonStyle(Qt::ToolButtonFollowStyle);
//    setStyleSheet("#" + objectName() + "{ background-color: rgba(212, 212, 212, 92); }");

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    QLabel * titleLabel = new QLabel(this);
    titleLabel -> setStyleSheet(QStringLiteral("color: white;"));
    QFont f = titleLabel -> font();
    f.setBold(true);
    f.setPixelSize(10);
    titleLabel -> setFont(f);
    QString row = titleLabel -> fontMetrics().elidedText(title, Qt::ElideRight, 30, Qt::TextWrapAnywhere);
    if (row.size() < title.size()) {
        row.chop(1);
        row = row + title.at(row.size()) % QStringLiteral("\n") % title.mid(row.length() + 1, row.length()) + ((title.size() > (row.length() * 2) + 1) ? QStringLiteral("..") : QStringLiteral(""));
    }

    titleLabel -> setText(row);
    titleLabel -> setMinimumSize(30, 30);
    titleLabel -> setMaximumSize(30, 30);
    titleLabel -> setAlignment(Qt::AlignCenter);
    titleLabel -> setContentsMargins(0,0,0,0);
    addWidget(titleLabel) -> setObjectName(QStringLiteral("*Title"));
}

ToolBar::~ToolBar() {
}

void ToolBar::dragEnterEvent(QDragEnterEvent * event) {
   if (event -> mimeData() -> hasFormat(QStringLiteral("text/uri-list")))
       event -> accept();
   else
       event -> ignore();
}

void ToolBar::dropEvent(QDropEvent * event) {
    if(event -> mimeData() -> hasUrls()) {
        QList<QUrl> list = event -> mimeData() -> urls();

        for(QList<QUrl>::Iterator url = list.begin(); url != list.end(); url++) {
            if ((*url).isLocalFile()) {
                QFileInfo file = QFileInfo((*url).toLocalFile());
                if (file.isDir()) {
                    emit folderDropped((file.baseName().isEmpty() ? file.filePath() : file.baseName()), file.filePath());
                }
            }
        }
        event -> accept();
    } else { event -> ignore(); }
}
