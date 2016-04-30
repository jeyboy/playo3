#ifndef WEB_APIS
#define WEB_APIS

#include "apis/storage/_storages.h"
#include "apis/site/_sites.h"
#include "apis/social/_socials.h"
#include "apis/service/_services.h"
#include "modules/core/core_parts/playlist.h"

namespace Core {
    namespace Web {
        class Apis {
            static QHash<DataSubType, ISource *> sources;
            static QHash<DataSubType, ISearchable *> searchers;
            static QHash<DataSubType, Sociable *> sociables;
            static QHash<DataSubType, IShareable *> shareables;
        public:
            static void initiate(const QJsonObject & obj);

            static QHash<DataSubType, ISearchable *> searchersList() { return searchers; }
            inline static ISearchable * searcher(const DataSubType & item_type) { return searchers.value(item_type); }

            static QHash<DataSubType, Sociable *> sociablesList() { return sociables; }
            inline static Sociable * sociable(const DataSubType & item_type) { return sociables.value(item_type); }

            static QHash<DataSubType, IShareable *> shareablesList() { return shareables; }
            inline static IShareable * shareable(const DataSubType & item_type) { return shareables.value(item_type); }

            inline static ISource * source(const DataSubType & item_type) { return sources.value(item_type); }

            static void close(QJsonObject & obj);

            static QString restoreUrl(const QString & refreshStr, const DataSubType & itemSubType) {
                qDebug() << "RESTORING" << itemSubType << refreshStr;

                ISource * source = sources.value(itemSubType);
                if (source == 0)
                    return QString();
                else
                    return source -> refresh(refreshStr);
            }
        };
    }
}

#endif // WEB_APIS
