#ifndef FOURSHARED_API
#define FOURSHARED_API

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/auth/teu_auth.h"
//#include "modules/core/web/interfaces/friendable.h"
//#include "modules/core/web/interfaces/groupable.h"
#include "modules/core/interfaces/isource.h"

#include "fourshared_request_api.h"

namespace Core {
    namespace Web {
        namespace Fourshared { // auth is not realised yet
            class Api: public ISource, public TeuAuth, public RequestApi, public Singleton<Api> {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { }
            public:
                inline QString name() const { return name_key; }
                inline DataSubType siteType() const { return dt_site_fourshared; }
                inline QUrlQuery genDefaultParams() {
                    return QUrlQuery(oauth_consumer_key % (token().isEmpty() ? def_token_key : token()));
                }

                void fromJson(const QJsonObject & hash);
                void toJson(QJsonObject & hash);

                inline bool isConnected() { return true/*!token().isEmpty()*/; }

                QString refresh(const QString & refresh_page) {
                    if (refresh_page.isEmpty()) return QString();

                    Html::Document doc = Web::Manager::prepare() -> followedGet(refresh_page) -> toHtml();
                    return doc.find("input.jsD1PreviewUrl").value();
                }

                QString downloadLink(const QString & refresh_page) {
                    if (refresh_page.isEmpty()) return QString();

                    Html::Document doc = Web::Manager::prepare() -> followedGet(QUrl(down_base_url % refresh_page.mid(12))) -> toHtml();
                    return doc.find("a[href~'/download/']").link();
                }

            public slots:
                inline void disconnect() { clearParams(); }

            protected:
                inline QString baseUrlStr(const QString & predicate) { return base_url % predicate % json_ext_key; }

//                inline QString offsetKey() const { return offset_key; }
//                inline QString limitKey() const { return limit_key; }
//                inline int requestLimit() const { return 100; }

                inline QJsonObject & extractBody(QJsonObject & response) { return response; }
                inline bool endReached(QJsonObject & response, int /*offset*/) { return response.value(files_token_key).toArray().isEmpty(); }
                inline bool extractStatus(QUrl & /*url*/, QJsonObject & /*response*/, int & /*code*/, QString & /*message*/) { return true; } // stub
            };
        }
    }
}

#endif // FOURSHARED_API
