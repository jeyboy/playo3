#include "zaycev.h"

using namespace Core::Web;

Zaycev * Zaycev::self = 0;

Zaycev * Zaycev::instance() {
    if(!self) self = new Zaycev();
    return self;
}
