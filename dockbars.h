#ifndef DOCKBARS_H
#define DOCKBARS_H

#include <QDockWidget>
#include <QMainWindow>
#include <QBoxLayout>

#include "modules/json/data_store.h"

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

        void load(QMainWindow * window, QJsonArray & bars);
        void save(QMainWindow * window, DataStore * settings);

        QDockWidget * createDocBar(QString name, QWidget * content, Qt::DockWidgetArea place = Qt::RightDockWidgetArea);
    private:
        Dockbars(QWidget * parent) : QWidget(parent) {

        }

        static Dockbars *self;
    };
}

#endif // DOCKBARS_H
