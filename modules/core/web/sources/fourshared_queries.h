#ifndef FOURSHARED_API
#define FOURSHARED_API

#include "modules/core/interfaces/singleton.h"

#include "fourshared/fourshared_requests.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Queries: public Requests, public Singleton<Queries> {
                Q_OBJECT

                friend class Singleton<Queries>;
                inline Queries() { }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_fourshared; }

                inline void userInfo(QString & uid, Func * func) { ThreadUtils::obj().run(this, &Queries::userInfo, uid, func); }
                QJsonArray userInfo(QString & uid) { return loadSetData(CMD_ID % '=' % uid).toArray(); }


//                QString downloadLink(const QString & refresh_page) {
//                    if (refresh_page.isEmpty()) return QString();

//                    Html::Document doc = Web::Manager::prepare() -> getFollowed(QUrl(url_down_base % refresh_page.mid(12))) -> toHtml();
//                    return doc.find("a[href~'/download/']").link();
//                }
            };
        }
    }
}

#endif // FOURSHARED_API
