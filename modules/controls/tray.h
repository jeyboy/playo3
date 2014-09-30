#ifndef TRAY_H
#define TRAY_H

#include <QSystemTrayIcon>

namespace Controls {
    class Tray : public QObject {
        Q_OBJECT
    public:
        ~Tray() {}

        static Tray * instance(QObject * parent);
        static void close() {
            m_tray.hide();
            delete self;
        }


    private:
        Tray(QObject * parent) : QObject(parent) {
            m_tray.setIcon(QIcon(":/icon"));

            if(m_tray.isSystemTrayAvailable()) {
        //        m_tray.showMessage();
                QMenu *pTrayMenu = new QMenu();
        //        pTrayMenu->addAction("Add snippet", this, SLOT(onAddSnippet()),Qt::ControlModifier + Qt::Key_D);
        //        pTrayMenu->addAction("Search", this, SLOT(onSearch()),Qt::MetaModifier + Qt::Key_V);
        //        pTrayMenu->addSeparator();
        //        pTrayMenu->addAction("Save", this, SLOT(save()));
        //        pTrayMenu->addSeparator();
        //        pTrayMenu->addAction("Exit", this, SLOT(exit()));
                m_tray.setContextMenu(pTrayMenu);
                m_tray.show();
            }
        }

        static Tray *self;
        QSystemTrayIcon m_tray;
    };
}

#endif // TRAY_H
