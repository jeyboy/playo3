#ifndef TRAY_H
#define TRAY_H

#include <qsystemtrayicon.h>
#include <qmenu.h>

namespace Controls {
    class Tray : public QSystemTrayIcon {
        Q_OBJECT
    public:
        Tray(QWidget * parent) : QSystemTrayIcon(parent) {
            setIcon(QIcon(":/ico"));

            if(isSystemTrayAvailable()) {
        //        m_tray.showMessage();
                QMenu * pTrayMenu = new QMenu(parent);
        //        pTrayMenu->addAction("Add snippet", this, SLOT(onAddSnippet()),Qt::ControlModifier + Qt::Key_D);
        //        pTrayMenu->addAction("Search", this, SLOT(onSearch()),Qt::MetaModifier + Qt::Key_V);
        //        pTrayMenu->addSeparator();
        //        pTrayMenu->addAction("Save", this, SLOT(save()));
        //        pTrayMenu->addSeparator();
        //        pTrayMenu->addAction("Exit", this, SLOT(exit()));
                setContextMenu(pTrayMenu);
                show();
            }
        }

        inline ~Tray() { hide(); }
    };
}

#endif // TRAY_H
