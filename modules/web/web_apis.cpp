#include "web_apis.h"

namespace Web {
    QList<ISearchable *> Apis::sites = QList<ISearchable *>();

    QList<ISearchable *> Apis::list() {
        if (sites.isEmpty()) {
            sites << Vk::Api::instance();
            sites << Soundcloud::Api::instance();
            sites << Fourshared::Api::instance();

            sites << Grabber::MyzukaAlbum::instance();
            sites << Grabber::Zaycev::instance();
            sites << Grabber::Mp3Base::instance();
            sites << Grabber::PromoDj::instance();
            sites << Grabber::Mp3cc::instance();
            sites << Grabber::Mp3pm::instance();
            sites << Grabber::Shmidt::instance();
            sites << Grabber::Jetune::instance();
        }

        return sites;
    }
}
