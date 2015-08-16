#include "shmidt.h"

namespace Grabber {
    Shmidt * Shmidt::self = 0;

    Shmidt * Shmidt::instance() {
        if(!self) self = new Shmidt();
        return self;
    }
}
