#ifndef WEB_APIS
#define WEB_APIS

#include "apis/storage/_storages.h"
#include "apis/site/_sites.h"
#include "apis/social/_socials.h"
#include "modules/core/core_parts/playlist.h"

namespace Core {
    namespace Web {
        class Apis {
            static QMap<DataSubType, ISource *> sources;
            static QMap<DataSubType, ISearchable *> searchers;
            static QMap<DataSubType, Sociable *> sociables;
            static QMap<DataSubType, IShareable *> shareables;
        public:
            static void initiate(const QJsonObject & obj);

            static QMap<DataSubType, ISearchable *> searchersList() { return searchers; }
            inline static ISearchable * searcher(const DataSubType & item_type) { return searchers.value(item_type); }

            static QMap<DataSubType, Sociable *> sociablesList() { return sociables; }
            inline static Sociable * sociable(const DataSubType & item_type) { return sociables.value(item_type); }
//            inline static bool isSociable(const DataSubType & item_type) { return sociables.contains(item_type); }

            static QMap<DataSubType, IShareable *> shareablesList() { return shareables; }
            inline static IShareable * shareable(const DataSubType & item_type) { return shareables.value(item_type); }

            static QMap<DataSubType, ISource *> sourcesList() { return sources; }
            inline static ISource * source(const DataSubType & item_type) { return sources.value(item_type); }

            static void close(QJsonObject & obj);

            static QString restoreUrl(const QString & refreshStr, const DataSubType & itemSubType) {
                qDebug() << "RESTORING" << itemSubType << refreshStr;

                ISource * source = sources.value(itemSubType);
                if (source == 0 || !source -> isConnected())
                    return QString();
                else
                    return source -> refresh(refreshStr);
            }
        };
    }
}

#endif // WEB_APIS
