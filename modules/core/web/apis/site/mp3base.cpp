#include "mp3base.h"

using namespace Core::Web;

Mp3Base * Mp3Base::self = 0;

Mp3Base * Mp3Base::instance() {
    if(!self) self = new Mp3Base();
    return self;
}
