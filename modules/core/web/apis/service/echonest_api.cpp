#include "echonest_api.h"

EchonestApi * EchonestApi::self = 0;

EchonestApi * EchonestApi::instance() {
    if (!self) self = new EchonestApi();
    return self;
}
