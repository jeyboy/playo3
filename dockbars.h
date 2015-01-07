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

        DockBar * createDocBar(QString name, QWidget * content, Qt::DockWidgetArea place = Qt::RightDockWidgetArea);
    private slots:
        void activeChanged();
    private:
        Dockbars(QWidget * parent) : QWidget(parent) {

        }

        static Dockbars *self;
    };
}

#endif // DOCKBARS_H
