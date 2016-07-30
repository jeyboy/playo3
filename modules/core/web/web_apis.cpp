#include "web_apis.h"

namespace Core {
    namespace Web {
        QMap<DataSubType, ISource *> Apis::sources = QMap<DataSubType, ISource *>();

        void Apis::initiate(const QJsonObject & obj) {
            sources.insert(Youtube::Api::obj().siteType(),      &Youtube::Api::obj());
            sources.insert(Yandex::Api::obj().siteType(),       &Yandex::Api::obj());
            sources.insert(Vk::Queries::obj().siteType(),           &Vk::Queries::obj());
            sources.insert(Soundcloud::Queries::obj().siteType(),   &Soundcloud::Queries::obj());
            sources.insert(Fourshared::Queries::obj().siteType(),   &Fourshared::Queries::obj());
            sources.insert(Od::Queries::obj().siteType(),           &Od::Queries::obj());
            sources.insert(MyzukaAlbum::obj().siteType(),       &MyzukaAlbum::obj());
            sources.insert(Zaycev::obj().siteType(),            &Zaycev::obj());
            sources.insert(Mp3Base::obj().siteType(),           &Mp3Base::obj());
            sources.insert(PromoDj::obj().siteType(),           &PromoDj::obj());
            sources.insert(Mp3cc::obj().siteType(),             &Mp3cc::obj());
            sources.insert(Mp3pm::obj().siteType(),             &Mp3pm::obj());
            sources.insert(Shmidt::obj().siteType(),            &Shmidt::obj());
            sources.insert(Jetune::obj().siteType(),            &Jetune::obj());
            sources.insert(MusicShara::obj().siteType(),        &MusicShara::obj());
            sources.insert(RedMp3::obj().siteType(),            &RedMp3::obj());

            for(QMap<DataSubType, ISource *>::Iterator it = sources.begin(); it != sources.end(); it++)
                it.value() -> fromJson(obj);
        }

        void Apis::close(QJsonObject & obj) {
            for(QMap<DataSubType, ISource *>::Iterator it = sources.begin(); it != sources.end(); it++)
                it.value() -> toJson(obj);
        }
    }
}
