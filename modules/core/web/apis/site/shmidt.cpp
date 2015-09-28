#include "shmidt.h"

using namespace Core::Web;

Shmidt * Shmidt::self = 0;

Shmidt * Shmidt::instance() {
    if(!self) self = new Shmidt();
    return self;
}
