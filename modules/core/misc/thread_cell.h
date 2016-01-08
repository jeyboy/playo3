#ifndef THREAD_CELL
#define THREAD_CELL

#include <qfuturewatcher.h>
#include <qdebug.h>

#include "func.h"

namespace Core {
    class Cell {
        protected:
            Func * response;
        public:
            Cell() : response(0) {}
            Cell(Func * func) : response(func) {}
            ~Cell() { delete response; }

            virtual void postprocessing(QObject * /*obj*/) {}
    };

    template<class T>
    struct WatchCell : public Cell {

        WatchCell(Func * func) : Cell(func) {}
        void postprocessing(QObject * obj) {
            QFutureWatcher<T> * initiator = (QFutureWatcher<T> *)obj;
            if (response -> obj && !initiator -> isCanceled() && response) {
                T result = initiator -> result();
                QGenericArgument arg = QArgument<T>(response -> arg, result);
                QMetaObject::invokeMethod(response -> obj, response -> slot, Qt::AutoConnection, arg);
            }
        }
    };

    template<>
    struct WatchCell<void> : public Cell {
        WatchCell(Func * func) : Cell(func) {}
        void postprocessing(QObject * obj) {
            QFutureWatcher<void> * initiator = (QFutureWatcher<void> *)obj;
                if (response -> obj && !initiator -> isCanceled() && response) {
                    QMetaObject::invokeMethod(response -> obj, qPrintable(response -> slot), Qt::AutoConnection);
                }
        }
    };

}

#endif // THREAD_CELL
