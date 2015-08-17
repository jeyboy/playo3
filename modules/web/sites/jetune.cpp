#include "jetune.h"

namespace Grabber {
    Jetune * Jetune::self = 0;

    Jetune * Jetune::instance() {
        if(!self) self = new Jetune();
        return self;
    }
}
