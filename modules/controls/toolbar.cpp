#include "toolbar.h"

#include <qlabel.h>
#include <qaction.h>
#include <qtoolbutton.h>
#include <qdebug.h>

using namespace Controls;

ToolBar::ToolBar(const QString & title, QWidget * parent) : QToolBar(title, parent), m_action_expand(0) {
    setAcceptDrops(true);
    setObjectName(QLatin1String("tool_") % title);
    setToolButtonStyle(Qt::ToolButtonFollowStyle);

    setAttribute(Qt::WA_NoSystemBackground, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    addTitleLabel(title);
    findExtension();
}

void ToolBar::addTitleLabel(const QString & title) {
    QLabel * titleLabel = new QLabel(this);
    titleLabel -> setProperty("colored_text", true);
    QFont f = titleLabel -> font();
    f.setBold(true);
    f.setPixelSize(10);
    titleLabel -> setFont(f);
    QString row = titleLabel -> fontMetrics().elidedText(title, Qt::ElideRight, 30, Qt::TextWrapAnywhere);
    if (row.size() < title.size()) {
        row.chop(1);
        row = row + title.at(row.size()) % QLatin1String("\n") % title.mid(row.length() + 1, row.length()) + ((title.size() > (row.length() * 2) + 1) ? QLatin1String("..") : QLatin1String(""));
    }

    titleLabel -> setText(row);
    titleLabel -> setMinimumSize(30, 30);
    titleLabel -> setMaximumSize(30, 30);
    titleLabel -> setAlignment(Qt::AlignCenter);
    titleLabel -> setContentsMargins(0,0,0,0);
    addWidget(titleLabel) -> setObjectName(QLatin1String("*Title"));
}

void ToolBar::findExtension() {
    for(QWidget * widget : findChildren<QWidget *>()) {
        if(widget -> objectName() == "qt_toolbar_ext_button") {
            m_action_expand = widget;
            return;
        }
    }
}

void ToolBar::forceExtensionClick(bool open) {
    if (!m_action_expand || m_action_expand -> isHidden()) return;
    QToolButton * btn = ((QToolButton *)m_action_expand);
    if (open != btn -> isDown())
        btn -> animateClick();
}

void ToolBar::dragEnterEvent(QDragEnterEvent * event) {
    if (event -> mimeData() -> hasFormat(QLatin1String("text/uri-list"))) {
        event -> accept();
        forceExtensionClick();
    } else
        event -> ignore();
}

void ToolBar::dragLeaveEvent(QDragLeaveEvent * event) {
    if (!rect().contains(mapFromGlobal(QCursor::pos()), true))
        forceExtensionClick(false);
    QToolBar::dragLeaveEvent(event);
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

    forceExtensionClick(false);
}
