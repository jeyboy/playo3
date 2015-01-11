#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <QToolBar>
#include <QMimeData>
#include <QMouseEvent>
#include <QFileInfo>
#include <qdebug.h>

namespace Playo3 {
    class ToolBar : public QToolBar {
        Q_OBJECT

    public:
        ToolBar(const QString &title, QWidget *parent = 0);
        ~ToolBar();

    protected:
        void dropEvent(QDropEvent *event);
        void dragEnterEvent(QDragEnterEvent *event);
        bool event(QEvent * ev) {
            emit(eventTriggered(ev));
            return QToolBar::event(ev);
        }
    signals:
        void eventTriggered(QEvent * ev);
        void folderDropped(QString name, QString path);
    };
}

#endif // TOOLBAR_H
