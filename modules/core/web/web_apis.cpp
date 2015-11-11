#include "web_apis.h"

namespace Core {
    namespace Web {
        QHash<SubType, ISearchable *> Apis::sites = QHash<SubType, ISearchable *>();

        void Apis::initiate(const QJsonObject & obj) {
            sites.insert(Vk::Api::obj().siteType(), &Vk::Api::obj());
            sites.insert(Soundcloud::Api::obj().siteType(), &Soundcloud::Api::obj());
            sites.insert(Fourshared::Api::obj().siteType(), &Fourshared::Api::obj());
            sites.insert(Od::Api::obj().siteType(), &Od::Api::obj());
            sites.insert(MyzukaAlbum::obj().siteType(), &MyzukaAlbum::obj());
            sites.insert(Zaycev::obj().siteType(), &Zaycev::obj());
            sites.insert(Mp3Base::obj().siteType(), &Mp3Base::obj());
            sites.insert(PromoDj::obj().siteType(), &PromoDj::obj());
            sites.insert(Mp3cc::obj().siteType(), &Mp3cc::obj());
            sites.insert(Mp3pm::obj().siteType(), &Mp3pm::obj());
            sites.insert(Shmidt::obj().siteType(), &Shmidt::obj());
            sites.insert(Jetune::obj().siteType(), &Jetune::obj());
            sites.insert(MusicShara::obj().siteType(), &MusicShara::obj());

            for(QHash<SubType, ISearchable *>::Iterator it = sites.begin(); it != sites.end(); it++)
                it.value() -> fromJson(obj);
        }

        void Apis::close(QJsonObject & obj) {
            for(QHash<SubType, ISearchable *>::Iterator it = sites.begin(); it != sites.end(); it++)
                it.value() -> toJson(obj);
        }
    }
}
