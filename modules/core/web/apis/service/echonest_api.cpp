#include "echonest_api.h"

using namespace Core::Web::Echonest;

Api * Api::self = 0;

Api * Api::instance() {
    if (!self) self = new Api();
    return self;
}
