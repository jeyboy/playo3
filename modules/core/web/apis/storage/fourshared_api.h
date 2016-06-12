#ifndef FOURSHARED_API
#define FOURSHARED_API

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/auth/oauth.h"

#include "fourshared_request_api.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Api: public RequestApi, public Singleton<Api> {
                Q_OBJECT

                friend class Singleton<Api>;
                inline Api() { }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_fourshared; }
                inline QUrlQuery genDefaultParams(const QueryParamsType & /*ptype*/ = json) {
                    return QUrlQuery(tkn_oauth_consumer % (token().isEmpty() ? val_token : token()));
                }
                inline SourceFlags defaultFlags() { return (SourceFlags)(sf_auth_api_has | sf_auth_site_has); }

                void fromJson(const QJsonObject & hash);
                void toJson(QJsonObject & hash);

//                QToolButton * initButton(QWidget * parent = 0);

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

//                    bool res = connectApi();
//                    res &= connectSite();

//                    return res;

                    return false;
                }
                inline void disconnectUser() {
                    clearParams();
                    initButton();
                }

            protected:
//                bool connectApi() {
////                    OAuth auth("22abeb63487b7f6b75051079b7e610b1", "71970e08961f3a78e821f51f989e6cb568cbd0ce");
////                    bool res = auth.initiate(url_api_base.arg(val_version) % QStringLiteral("oauth/initiate"));
////                    if (res)
////                        res = auth.autorize(url_api_base.arg(val_version) % QStringLiteral("oauth/authorize"));
////                    if (res)
////                        res = auth.access(url_api_base.arg(val_version) % QStringLiteral("oauth/token"));

////                    return res;

////                    OAuth auth("i0SEyiZts1mMGizAVDjn4nhOH", "O52MLLcKA0LknMLf47ZKyvTQhwdhLshsSY865hEgERS6ediuSj", QStringLiteral("http://localhost:3000/"));
////                    bool res = auth.initiate(QStringLiteral("https://api.twitter.com/oauth/request_token"));
////                    if (res)
////                        res = auth.autorize(QStringLiteral("https://api.twitter.com/oauth/authenticate"));
////                    if (res)
////                        res = auth.access(QStringLiteral("https://api.twitter.com/oauth/access_token"));

//                        APPEND_FLAG(flags, sf_auth_api_done);

//                    return res;

//                    return true;
//                }

//                bool connectSite() {
//                    APPEND_FLAG(flags, sf_auth_site_done);

//                    return false;
//                }

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
