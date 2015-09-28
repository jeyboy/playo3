#include "mp3pm.h"

using namespace Core::Web;

Mp3pm * Mp3pm::self = 0;

Mp3pm * Mp3pm::instance() {
    if(!self) self = new Mp3pm();
    return self;
}
