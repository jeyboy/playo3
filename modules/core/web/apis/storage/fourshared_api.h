#ifndef FOURSHARED_API
#define FOURSHARED_API

#include "modules/core/interfaces/singleton.h"
#include "modules/core/interfaces/isource.h"
#include "modules/core/web/interfaces/auth/oauth.h"

#include "fourshared_request_api.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Api: public RequestApi, public ISource, public Singleton<Api> {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_fourshared; }
                inline QUrlQuery genDefaultParams(const QueryParamsType & /*ptype*/ = json) {
                    return QUrlQuery(tkn_oauth_consumer % (token().isEmpty() ? val_token : token()));
                }

                void fromJson(const QJsonObject & hash);
                void toJson(QJsonObject & hash);

                inline bool isConnected() { return !token().isEmpty(); }
                QToolButton * initButton(QWidget * parent = 0);

                QString refresh(const QString & refresh_page) {
                    Html::Document doc = Web::Manager::prepare() -> followedGet(refresh_page) -> toHtml();
                    return doc.find("input.jsD1PreviewUrl").value();
                }

                QString downloadLink(const QString & refresh_page) {
                    if (refresh_page.isEmpty()) return QString();

                    Html::Document doc = Web::Manager::prepare() -> followedGet(QUrl(url_down_base % refresh_page.mid(12))) -> toHtml();
                    return doc.find("a[href~'/download/']").link();
                }
            public slots:
                inline void openTab() { ISource::openTab(userID()); }
                bool connectUser(const ConnectionType & /*conType*/ = connection_restore) {
                    if (isConnected()) return true;

                    OAuth auth("22abeb63487b7f6b75051079b7e610b1", "71970e08961f3a78e821f51f989e6cb568cbd0ce");
                    bool res = auth.initiatePost(QStringLiteral("https://api.4shared.com/v1_2/oauth/initiate"));
                    if (res)
                        auth.autorize(QStringLiteral("https://api.4shared.com/v1_2/oauth/authorize"));

                    int i = 0;
                }
                inline void disconnectUser() {
                    clearParams();
                    initButton();
                }

            protected:
                inline QString baseUrlStr(const QString & predicate) { return url_api_base.arg(val_version) % predicate % val_json_ext; }

                bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply = false) {
                    int i = 0;
                    //TODO: realize me
                }

                inline bool endReached(QJsonObject & response, QueriableArg * arg) { return response.value(tkn_files).toArray().size() < arg -> per_request_limit; }
                inline bool extractStatus(QueriableArg * /*arg*/, QJsonObject & /*json*/, int & /*code*/, QString & /*message*/) { return true; } // stub
            };
        }
    }
}

#endif // FOURSHARED_API
