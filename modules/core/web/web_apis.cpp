#include "web_apis.h"

namespace Core {
    namespace Web {
        QMap<DataSubType, ISource *> Apis::sources = QMap<DataSubType, ISource *>();
        QMap<DataSubType, ISearchable *> Apis::searchers = QMap<DataSubType, ISearchable *>();
        QMap<DataSubType, Sociable *> Apis::sociables = QMap<DataSubType, Sociable *>();
        QMap<DataSubType, IShareable *> Apis::shareables = QMap<DataSubType, IShareable *>();

        void Apis::initiate(const QJsonObject & obj) {
            sources.insert(Youtube::Api::obj().siteType(),      &Youtube::Api::obj());
            sources.insert(Yandex::Api::obj().siteType(),       &Yandex::Api::obj());
            sources.insert(Vk::Api::obj().siteType(),           &Vk::Api::obj());
            sources.insert(Soundcloud::Api::obj().siteType(),   &Soundcloud::Api::obj());
            sources.insert(Fourshared::Api::obj().siteType(),   &Fourshared::Api::obj());
            sources.insert(Od::Api::obj().siteType(),           &Od::Api::obj());
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

            for(QMap<DataSubType, ISource *>::Iterator it = sources.begin(); it != sources.end(); it++) {

                ISearchable * ser = dynamic_cast<ISearchable *>(it.value());
                if (ser) searchers.insert(it.key(), ser);

                Sociable * soc = dynamic_cast<Sociable *>(it.value());
                if (soc) sociables.insert(it.key(), soc);

                IShareable * shar = dynamic_cast<IShareable *>(it.value());
                if (shar) shareables.insert(it.key(), shar);

                it.value() -> fromJson(obj);
            }

            qDebug() << "SOURCES: " << sources.size();
            qDebug() << "SEARCHERS: " << searchers.size();
            qDebug() << "SOCIABLES: " << sociables.size();
            qDebug() << "SHAREABLES: " << shareables.size();
            qDebug() << "------------------------------------------";
        }

        void Apis::close(QJsonObject & obj) {
            for(QMap<DataSubType, ISource *>::Iterator it = sources.begin(); it != sources.end(); it++)
                it.value() -> toJson(obj);
        }
    }
}
