#include "web_apis.h"

namespace Core {
    namespace Web {
        QHash<DataSubType, ISource *> Apis::sources = QHash<DataSubType, ISource *>();
        QHash<DataSubType, ISearchable *> Apis::searchers = QHash<DataSubType, ISearchable *>();
        QHash<DataSubType, Sociable *> Apis::sociables = QHash<DataSubType, Sociable *>();
        QHash<DataSubType, IShareable *> Apis::shareables = QHash<DataSubType, IShareable *>();

        void Apis::initiate(const QJsonObject & obj) {
            sources.insert(Youtube::Api::obj().siteType(), &Youtube::Api::obj());
            sources.insert(Yandex::Api::obj().siteType(), &Yandex::Api::obj());
            sources.insert(Vk::Api::obj().siteType(), &Vk::Api::obj());
            sources.insert(Soundcloud::Api::obj().siteType(), &Soundcloud::Api::obj());
            sources.insert(Fourshared::Api::obj().siteType(), &Fourshared::Api::obj());
            sources.insert(Od::Api::obj().siteType(), &Od::Api::obj());
            sources.insert(MyzukaAlbum::obj().siteType(), &MyzukaAlbum::obj());
            sources.insert(Zaycev::obj().siteType(), &Zaycev::obj());
            sources.insert(Mp3Base::obj().siteType(), &Mp3Base::obj());
            sources.insert(PromoDj::obj().siteType(), &PromoDj::obj());
            sources.insert(Mp3cc::obj().siteType(), &Mp3cc::obj());
            sources.insert(Mp3pm::obj().siteType(), &Mp3pm::obj());
            sources.insert(Shmidt::obj().siteType(), &Shmidt::obj());
            sources.insert(Jetune::obj().siteType(), &Jetune::obj());
            sources.insert(MusicShara::obj().siteType(), &MusicShara::obj());
            sources.insert(RedMp3::obj().siteType(), &RedMp3::obj());

            for(QHash<DataSubType, ISource *>::Iterator it = sources.begin(); it != sources.end(); it++) {
                if (it.value() -> isSearchable())
                    searchers.insert(it.key(), (ISearchable *)it.value());

                if (it.value() -> isSociable())
                    sociables.insert(it.key(), (Sociable *)it.value());

                if (it.value() -> isShareable())
                    shareables.insert(it.key(), (IShareable *)it.value());

                it.value() -> fromJson(obj);
            }
        }

        void Apis::close(QJsonObject & obj) {
            for(QHash<DataSubType, ISource *>::Iterator it = sources.begin(); it != sources.end(); it++)
                it.value() -> toJson(obj);
        }
    }
}
