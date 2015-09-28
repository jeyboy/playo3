#include "promodj.h"

using namespace Core::Web;

PromoDj * PromoDj::self = 0;

PromoDj * PromoDj::instance() {
    if(!self) self = new PromoDj();
    return self;
}
