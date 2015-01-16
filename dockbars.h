#ifndef DOCKBARS_H
#define DOCKBARS_H

#include <QDockWidget>
#include <QMainWindow>
#include <QBoxLayout>

#include "misc/file_utils/data_store.h"
#include "modules/controls/dockbar.h"

namespace Playo3 {
    class Dockbars : public QWidget {
        Q_OBJECT
    public:
        ~Dockbars() {

        }

        static Dockbars * instance(QWidget * parent);
        static void close() {
            delete self;
        }

        static inline QString settingsName() { return "docks"; }

        void load(QJsonArray & bars);
        void save(DataStore * settings);
        QDockWidget * linkNameToToolbars(QString barName);

        inline QList<DockBar *> dockbars() { return parent() -> findChildren<DockBar *>(); }
        inline DockBar * current() const { return active; }

        DockBar * createDocBar(QString name, QWidget * content);
        inline void activate(DockBar * bar) {
            bar -> raise();
            active = bar;
        }
    public slots:
        void hideAll();
        void showAll();

    private slots:
        void activeChanged();
        void barClosed();
    private:
        DockBar * active;

        Dockbars(QWidget * parent) : QWidget(parent), active(0) {

        }

        static Dockbars *self;
    };
}

#endif // DOCKBARS_H
