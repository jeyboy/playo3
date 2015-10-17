#ifndef THREAD_UTILS
#define THREAD_UTILS

#include <qthread.h>
#include <qapplication.h>

namespace Core {
    class ThreadUtils {
    public:
        static bool inMainThread() {
            return QThread::currentThread() != QApplication::instance() -> thread();
        }

        static bool livesInCurrThread(QObject * receiver) {
            return QThread::currentThread() == receiver -> thread();
        }
    };
}

#endif // THREAD_UTILS
