#include "toolbar_drop_button.h"

using namespace Controls;

ToolbarDropButton::ToolbarDropButton(const QString & text, QWidget * parent) : QToolButton(parent) {
    setText(text);
    setAcceptDrops(true);
    /*setDefaultDropAction(Qt::MoveAction);*/
}

void ToolbarDropButton::dragEnterEvent(QDragEnterEvent * event) {
    if (event -> mimeData() -> hasFormat(QStringLiteral("text/uri-list")))
        event -> accept();
    else event -> ignore();
}

void ToolbarDropButton::dropEvent(QDropEvent * event) {
    dropReaction(event);
}
