#ifndef DOCKBARS_H
#define DOCKBARS_H

#include <QDockWidget>
#include <QMainWindow>
#include <QBoxLayout>

#include "modules/json/data_store.h"
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

        void load(QMainWindow * window, QJsonArray & bars);
        void save(QMainWindow * window, DataStore * settings);

        inline QList<QDockWidget *> dockbars() { return parent() -> findChildren<QDockWidget *>(); }
        inline DockBar * current() const { return active; }

        DockBar * addDocBar(QString name, QWidget * content, Qt::DockWidgetArea place = Qt::RightDockWidgetArea);
        inline void activate(DockBar * bar) {
            bar -> raise();
            active = bar;
        }
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
