#include "web_apis.h"

namespace Core {
    namespace Web {
        QHash<SubType, ISearchable *> Apis::sites = QHash<SubType, ISearchable *>();

        QHash<SubType, ISearchable *> Apis::list() {
            if (sites.isEmpty()) {
                sites.insert(Vk::Api::instance() -> siteType(), Vk::Api::instance());
                sites.insert(Soundcloud::Api::instance() -> siteType(), Soundcloud::Api::instance());
                sites.insert(Fourshared::Api::instance() -> siteType(), Fourshared::Api::instance());
                sites.insert(Od::Api::instance() -> siteType(), Od::Api::instance());

                sites.insert(MyzukaAlbum::instance() -> siteType(), MyzukaAlbum::instance());
                sites.insert(Zaycev::instance() -> siteType(), Zaycev::instance());
                sites.insert(Mp3Base::instance() -> siteType(), Mp3Base::instance());
                sites.insert(PromoDj::instance() -> siteType(), PromoDj::instance());
                sites.insert(Mp3cc::instance() -> siteType(), Mp3cc::instance());
                sites.insert(Mp3pm::instance() -> siteType(), Mp3pm::instance());
                sites.insert(Shmidt::instance() -> siteType(), Shmidt::instance());
                sites.insert(Jetune::instance() -> siteType(), Jetune::instance());
                sites.insert(MusicShara::instance() -> siteType(), MusicShara::instance());
            }

            return sites;
        }
    }
}
