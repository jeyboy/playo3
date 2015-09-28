#include "mp3cc.h"

using namespace Core::Web;

Mp3cc * Mp3cc::self = 0;

Mp3cc * Mp3cc::instance() {
    if(!self) self = new Mp3cc();
    return self;
}
