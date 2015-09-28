#include "musicshara.h"

using namespace Core::Web;

MusicShara * MusicShara::self = 0;

MusicShara * MusicShara::instance() {
    if(!self) self = new MusicShara();
    return self;
}
