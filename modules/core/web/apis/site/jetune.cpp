#include "jetune.h"

using namespace Core::Web;

Jetune * Jetune::self = 0;

Jetune * Jetune::instance() {
    if(!self) self = new Jetune();
    return self;
}
