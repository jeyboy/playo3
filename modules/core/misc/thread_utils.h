#ifndef THREAD_UTILS
#define THREAD_UTILS

//#include <typeinfo>
#include <qthread.h>
#include <qapplication.h>

#include <QtConcurrent/qtconcurrentrun.h>

#include "modules/core/interfaces/singleton.h"
#include "thread_cell.h"

namespace Core {
    class ThreadUtils : public QObject, public Singleton<ThreadUtils> {
        Q_OBJECT

        friend class Singleton<ThreadUtils>;

        QHash<QObject *, Cell *> requests;
    protected:
        template<typename T>
        void registerSync(QFuture<T> feature, Func * response) {
            QFutureWatcher<T> * initiator = new QFutureWatcher<T>();
            requests.insert(initiator, new WatchCell<T>(response));
            QObject::connect(initiator, SIGNAL(finished()), this, SLOT(postprocessing()));
            initiator -> setFuture(feature);
        }
    protected slots:
        void postprocessing() {
            QObject * obj = sender();
            requests[obj] -> postprocessing(obj);
            obj -> deleteLater();
        }
    public:
        static bool inMainThread() { return QThread::currentThread() == QApplication::instance() -> thread(); }
        static bool livesInCurrThread(QObject * receiver) { return QThread::currentThread() == receiver -> thread(); }

        template <typename T, typename Class>
        void run(Class * object, T (Class::*fn)(), Func * response) {
            registerSync(QtConcurrent::run(object, fn), response);
        }
        template <typename T, typename Class, typename Param1, typename Arg1>
        void run(Class * object, T (Class::*fn)(Param1), const Arg1 & arg1, Func * response) {
            registerSync(QtConcurrent::run(object, fn, arg1), response);
        }
        template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2>
        void run(Class * object, T (Class::*fn)(Param1, Param2), const Arg1 & arg1, const Arg2 & arg2, Func * response) {
            registerSync(QtConcurrent::run(object, fn, arg1, arg2), response);
        }
        template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3>
        void run(Class * object, T (Class::*fn)(Param1, Param2, Param3), const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, Func * response) {
            registerSync(QtConcurrent::run(object, fn, arg1, arg2, arg3), response);
        }
        template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4>
        void run(Class * object, T (Class::*fn)(Param1, Param2, Param3, Param4), const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, Func * response) {
            registerSync(QtConcurrent::run(object, fn, arg1, arg2, arg3, arg4), response);
        }
        template <typename T, typename Class, typename Param1, typename Arg1, typename Param2, typename Arg2, typename Param3, typename Arg3, typename Param4, typename Arg4, typename Param5, typename Arg5>
        void run(Class * object, T (Class::*fn)(Param1, Param2, Param3, Param4, Param5), const Arg1 & arg1, const Arg2 & arg2, const Arg3 & arg3, const Arg4 & arg4, const Arg5 & arg5, Func * response) {
            registerSync(QtConcurrent::run(object, fn, arg1, arg2, arg3, arg4, arg5), response);
        }
    };
}

#endif // THREAD_UTILS
