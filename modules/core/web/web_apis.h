#ifndef WEB_APIS
#define WEB_APIS

#include "apis/storage/_storages.h"
#include "apis/site/_sites.h"
#include "apis/social/_socials.h"
#include "apis/service/_services.h"

#define SETTINGS_VK_SET_KEY QStringLiteral("vk")
#define SETTINGS_SOUNDCLOUD_SET_KEY QStringLiteral("soundcloud")
#define SETTINGS_FOURSHARED_SET_KEY QStringLiteral("fourshared")
#define SETTINGS_OD_SET_KEY QStringLiteral("od")

namespace Core {
    namespace Web {
        class Apis {
            static QHash<Web::SubType, ISearchable *> sites;
        public:
            static void initiate(QWidget * parent, const QJsonObject & obj);
            static QHash<Web::SubType, ISearchable *> list() { return sites; }
            inline static ISearchable * engine(Web::SubType item_type) { return sites.value(item_type); }
            inline static void close(QJsonObject & obj) {
                obj.insert(SETTINGS_VK_SET_KEY, Vk::Api::instance() -> toJson());
                obj.insert(SETTINGS_SOUNDCLOUD_SET_KEY, Soundcloud::Api::instance() -> toJson());
                obj.insert(SETTINGS_FOURSHARED_SET_KEY, Fourshared::Api::instance() -> toJson());
                obj.insert(SETTINGS_OD_SET_KEY, Od::Api::instance() -> toJson());


                qDeleteAll(sites);
            }

        };
    }
}

#endif // WEB_APIS
