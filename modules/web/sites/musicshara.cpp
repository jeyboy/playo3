#include "musicshara.h"

namespace Grabber {
    MusicShara * MusicShara::self = 0;

    MusicShara * MusicShara::instance() {
        if(!self) self = new MusicShara();
        return self;
    }
}
