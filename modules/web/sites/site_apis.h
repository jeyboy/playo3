#ifndef SITE_APIS
#define SITE_APIS

#include "myzuka_album.h"
#include "zaycev.h"
#include "mp3base.h"
#include "promodj.h"
#include "mp3cc.h"
#include "mp3pm.h"
#include "shmidt.h"
#include "jetune.h"

namespace Grabber {
    class Apis {
    public:
        static QList<IGrabberApi *> list() const;
        inline static void close() {
            qDeleteAll(sites);
        }
    private:
        static QList<IGrabberApi *> sites;
    };
}

#endif // SITE_APIS
