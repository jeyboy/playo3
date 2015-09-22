#include "mp3base.h"

namespace Grabber {
    Mp3Base * Mp3Base::self = 0;

    Mp3Base * Mp3Base::instance() {
        if(!self) self = new Mp3Base();
        return self;
    }
}
