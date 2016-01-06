#include "watcher.h"

namespace {
    class X11Watcher : public QWidget, public Core::Singleton<X11Watcher> {
        public:
            X11Watcher() {}
            ~X11Watcher() {}

            bool registerPath(qintptr & id, const QString & path, bool recursive) {}
            void unregisterPath(qintptr ptr) {}
    };
}

