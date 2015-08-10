#include "zaycev.h"

namespace Grabber {
    Zaycev * Zaycev::self = 0;

    Zaycev * Zaycev::instance() {
        if(!self) self = new Zaycev();
        return self;
    }
}
