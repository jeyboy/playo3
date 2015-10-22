#include "web_apis.h"

namespace Core {
    namespace Web {
        QHash<SubType, ISearchable *> Apis::sites = QHash<SubType, ISearchable *>();

        void Apis::initiate(const QJsonObject & obj) {
            Vk::Api::obj().fromJson(obj.value(SETTINGS_VK_SET_KEY).toObject());
            sites.insert(Vk::Api::obj().siteType(), &Vk::Api::obj());

            Soundcloud::Api::obj().fromJson(obj.value(SETTINGS_SOUNDCLOUD_SET_KEY).toObject());
            sites.insert(Soundcloud::Api::obj().siteType(), &Soundcloud::Api::obj());

            sites.insert(Fourshared::Api::instance() -> siteType(), Fourshared::Api::instance(obj.value(SETTINGS_FOURSHARED_SET_KEY).toObject()));

            Od::Api::obj().fromJson(obj.value(SETTINGS_OD_SET_KEY).toObject());
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
        }
    }
}
