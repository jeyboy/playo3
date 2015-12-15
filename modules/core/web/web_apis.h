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
            static QHash<Web::SubType, ISearchable *> sites;
        public:
            static void initiate(const QJsonObject & obj);
            static QHash<Web::SubType, ISearchable *> list() { return sites; }
            inline static ISearchable * engine(const Web::SubType & item_type) { return sites.value(item_type); }
            static void close(QJsonObject & obj);

            static QString restoreUrl(const QString & refreshStr/*, int itemType*/, const Web::SubType & itemSubType) {
//                switch(itemType) {
////                    case VK_FILE: {
////                        newUrl = Vk::Api::obj().refresh(refreshStr /*itm -> toUid()*/).section('?', 0, 0);
////                    break;}

////                    case OD_FILE: {
////                        newUrl = Od::Api::obj().refresh(refreshStr/*itm -> refresh_path()*/);
////                    break;}

//                    case VK_FILE:
//                    case OD_FILE:
//                    case WEB_FILE: {
                        switch(itemSubType) {
            //                case Playo3::fourshared_site: {
            //                    newUrl = Fourshared::Api::instance() -> refresh(itm -> refresh_path());
            ////                    newUrl = Fourshared::Api::instance() -> downloadLink(itm -> refresh_path());
            //                break;}

            //                case jetune_site: {
            //                    newUrl = itm -> refresh_path();
            //                break;}

                            default: {
                                ISearchable * engine = Web::Apis::engine(itemSubType);
                                if (engine == 0)
                                    return QString();
                                else {
                                    QString res = engine -> refresh(refreshStr /*itm -> refresh_path()*/);
                                    if (itemSubType == vk_site) return res.section('?', 0, 0);
                                    else return res;
                                }
                            }
                        }
//                    break;}
//                    default: return QString();
//                };
            }
        };
    }
}

#endif // WEB_APIS
