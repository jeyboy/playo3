#include "promodj.h"

namespace Grabber {
    PromoDj * PromoDj::self = 0;

    PromoDj * PromoDj::instance() {
        if(!self) self = new PromoDj();
        return self;
    }
}
