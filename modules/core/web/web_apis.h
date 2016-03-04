#ifndef WEB_APIS
#define WEB_APIS

#include "apis/storage/_storages.h"
#include "apis/site/_sites.h"
#include "apis/social/_socials.h"
#include "apis/service/_services.h"
#include "modules/core/core_parts/core_part_types.h"

namespace Core {
    namespace Web {
        class Apis {
            static QHash<DataSubType, ISearchable *> sites;
        public:
            static void initiate(const QJsonObject & obj);
            static QHash<DataSubType, ISearchable *> list() { return sites; }
            inline static ISearchable * engine(const DataSubType & item_type) { return sites.value(item_type); }
            static void close(QJsonObject & obj);

            static QString restoreUrl(const QString & refreshStr/*, int itemType*/, const DataSubType & itemSubType) {
                qDebug() << "RESTORING" << itemSubType << refreshStr;

                ISearchable * engine = Web::Apis::engine(itemSubType);
                if (engine == 0)
                    return QString();
                else {
                    QString res = engine -> refresh(refreshStr /*itm -> refresh_path()*/);
                    if (itemSubType == dt_site_vk) return res.section('?', 0, 0);
                    else return res;
                }
            }
        };
    }
}

#endif // WEB_APIS
