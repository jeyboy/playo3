#include "filesystem_watcher.h"

namespace {   
    class MacWatcher : public QWidget, public Core::RelSingleton<MacWatcher> {
        public:
            MacWatcher() {}
            ~MacWatcher() {}

            bool registerPath(qintptr & id, const QString & path, bool recursive) {}
            void unregisterPath(qintptr ptr) {}
    };
}
