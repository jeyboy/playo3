#include "site_apis.h"

namespace Grabber {
    QList<IGrabberApi *> Apis::sites = QList<IGrabberApi *>();

    static QList<IGrabberApi *> Apis::list() const {
        if (sites.isEmpty()) {
            sites << MyzukaAlbum::instance();
            sites << Zaycev::instance();
            sites << Mp3Base::instance();
            sites << PromoDj::instance();
            sites << Mp3cc::instance();
            sites << Mp3pm::instance();
            sites << Shmidt::instance();
            sites << Jetune::instance();
        }

        return sites;
    }
}
