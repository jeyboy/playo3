#include "myzuka_album.h"

namespace Grabber {
    MyzukaAlbum * MyzukaAlbum::self = 0;

    MyzukaAlbum * MyzukaAlbum::instance() {
        if(!self) self = new MyzukaAlbum();
        return self;
    }
}
