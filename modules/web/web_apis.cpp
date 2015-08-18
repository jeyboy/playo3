#include "web_apis.h"

namespace Web {
    QHash<Playo3::WebSubType, ISearchable *> Apis::sites = QHash<Playo3::WebSubType, ISearchable *>();

    QHash<Playo3::WebSubType, ISearchable *> Apis::list() {
        if (sites.isEmpty()) {
            sites.insert(Vk::Api::instance() -> siteType(), Vk::Api::instance());
            sites.insert(Soundcloud::Api::instance() -> siteType(), Soundcloud::Api::instance());
            sites.insert(Fourshared::Api::instance() -> siteType(), Fourshared::Api::instance());

            sites.insert(Grabber::MyzukaAlbum::instance() -> siteType(), Grabber::MyzukaAlbum::instance());
            sites.insert(Grabber::Zaycev::instance() -> siteType(), Grabber::Zaycev::instance());
            sites.insert(Grabber::Mp3Base::instance() -> siteType(), Grabber::Mp3Base::instance());
            sites.insert(Grabber::PromoDj::instance() -> siteType(), Grabber::PromoDj::instance());
            sites.insert(Grabber::Mp3cc::instance() -> siteType(), Grabber::Mp3cc::instance());
            sites.insert(Grabber::Mp3pm::instance() -> siteType(), Grabber::Mp3pm::instance());
            sites.insert(Grabber::Shmidt::instance() -> siteType(), Grabber::Shmidt::instance());
            sites.insert(Grabber::Jetune::instance() -> siteType(), Grabber::Jetune::instance());
        }

        return sites;
    }
}
