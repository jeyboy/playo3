#include "mp3pm.h"

namespace Grabber {
    Mp3pm * Mp3pm::self = 0;

    Mp3pm * Mp3pm::instance() {
        if(!self) self = new Mp3pm();
        return self;
    }
}
