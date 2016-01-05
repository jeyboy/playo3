#include "watcher.h"

namespace {
    class MacWatcher : public QWidget, public WatcherCell {
        public:
            MacWatcher(const QString & /*path*/, bool /*recursive*/) {}
            ~MacWatcher() {}
    };
}
