#include "myzuka_album.h"

using namespace Core::Web;

MyzukaAlbum * MyzukaAlbum::self = 0;

MyzukaAlbum * MyzukaAlbum::instance() {
    if(!self) self = new MyzukaAlbum();
    return self;
}
