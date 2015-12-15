#include "toolbar_new_list_button.h"

#include <qfileinfo.h>

using namespace Controls;

ToolbarNewListButton::ToolbarNewListButton(const QIcon & ico, QWidget * parent) : ToolbarDropButton(QString(), parent) {
    setIcon(ico);
}

void ToolbarNewListButton::dropReaction(QDropEvent * event) {
    if(event -> mimeData() -> hasUrls()) {
        QList<QUrl> list = event -> mimeData() -> urls();

        for(QList<QUrl>::Iterator url = list.begin(); url != list.end(); url++) {
            if ((*url).isLocalFile()) {
                QFileInfo file = QFileInfo((*url).toLocalFile());
                if (file.isDir())
                    emit folderAdded((file.baseName().isEmpty() ? file.filePath() : file.baseName()), (*url));
            }
        }
        event -> accept();
    } else { event -> ignore(); }
}
