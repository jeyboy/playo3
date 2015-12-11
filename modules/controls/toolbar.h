#ifndef TOOLBAR_H
#define TOOLBAR_H

#include <qtoolbar.h>
#include <qmimedata.h>
#include <qevent.h>
#include <qfileinfo.h>
#include <qstringbuilder.h>

namespace Controls {
    class ToolBar : public QToolBar {
        Q_OBJECT

        QWidget * m_action_expand;
    public:
        ToolBar(const QString & title, QWidget * parent = 0);

    protected:
        void findExtension();
        void forceExtensionClick(bool open = true);
        void addTitleLabel(const QString & title);
        void dropEvent(QDropEvent * event);
        void dragEnterEvent(QDragEnterEvent * event);
        void dragLeaveEvent(QDragLeaveEvent * event);
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
