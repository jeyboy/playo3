#include "mp3cc.h"

namespace Grabber {
    Mp3cc * Mp3cc::self = 0;

    Mp3cc * Mp3cc::instance() {
        if(!self) self = new Mp3cc();
        return self;
    }
}
