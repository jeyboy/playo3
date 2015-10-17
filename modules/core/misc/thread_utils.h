#ifndef THREAD_UTILS
#define THREAD_UTILS

#include <qthread.h>
#include <qapplication.h>

namespace Core {
    class ThreadUtils {
    public:
        static bool inThread() {
            return QThread::currentThread() != QApplication::instance() -> thread();
        }
    };
}

#endif // THREAD_UTILS
