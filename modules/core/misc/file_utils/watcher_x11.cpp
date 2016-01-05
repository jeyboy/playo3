#include "watcher.h"

namespace {
    class X11Watcher : public QWidget, public WatcherCell {
        public:
            X11Watcher(const QString & /*path*/, bool /*recursive*/) {}
            ~X11Watcher() {}
    };
}

