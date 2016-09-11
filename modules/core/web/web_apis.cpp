#include "web_apis.h"

namespace Core {
    namespace Web {
        QMap<DataSubType, ISource *> Apis::sources = QMap<DataSubType, ISource *>();
        QMap<DataSubType, QString> Apis::sources_name = QMap<DataSubType, QString>();

        void Apis::initiate(const QJsonObject & obj) {
            sources.insert(Youtube::Api::obj().sourceType(),      &Youtube::Api::obj());
            sources.insert(Yandex::Queries::obj().sourceType(),       &Yandex::Queries::obj());
            sources.insert(Vk::Queries::obj().sourceType(),           &Vk::Queries::obj());
            sources.insert(Soundcloud::Queries::obj().sourceType(),   &Soundcloud::Queries::obj());
            sources.insert(Fourshared::Queries::obj().sourceType(),   &Fourshared::Queries::obj());
            sources.insert(Od::Queries::obj().sourceType(),           &Od::Queries::obj());
            sources.insert(MyzukaAlbum::obj().sourceType(),       &MyzukaAlbum::obj());
            sources.insert(Zaycev::obj().sourceType(),            &Zaycev::obj());
            sources.insert(Mp3Base::obj().sourceType(),           &Mp3Base::obj());
            sources.insert(PromoDj::obj().sourceType(),           &PromoDj::obj());
            sources.insert(Mp3cc::obj().sourceType(),             &Mp3cc::obj());
            sources.insert(Mp3pm::obj().sourceType(),             &Mp3pm::obj());
            sources.insert(Shmidt::obj().sourceType(),            &Shmidt::obj());
            sources.insert(Jetune::obj().sourceType(),            &Jetune::obj());
            sources.insert(MusicShara::obj().sourceType(),        &MusicShara::obj());
            sources.insert(RedMp3::obj().sourceType(),            &RedMp3::obj());

            for(QMap<DataSubType, ISource *>::Iterator it = sources.begin(); it != sources.end(); it++) {
                it.value() -> fromJson(obj);
                sources_name.insert(it.key(), it.value() -> name());
            }
        }

        void Apis::close(QJsonObject & obj) {
            for(QMap<DataSubType, ISource *>::Iterator it = sources.begin(); it != sources.end(); it++)
                it.value() -> toJson(obj);
        }
    }
}
