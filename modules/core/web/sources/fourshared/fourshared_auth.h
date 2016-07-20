#ifndef FOURSHARED_AUTH
#define FOURSHARED_AUTH

#include "fourshared_defines.h"

#include "modules/core/web/interfaces/auth/oauth.h"
#include "modules/core/interfaces/iuser_interaction.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Auth : public Base, public IUserInteraction {
            public:
                bool connectApi() {
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

//                    return res;

                    return false;
                }
                bool connectSite(QString & user_id, QString & err) {
                    Html::Document html = Manager::prepare() -> getFollowed(url_html_site_base) -> toHtml();

                    Html::Tag * form = html.findFirst("form[name='loginForm']");
                    if (!form) return false;
                    QHash<QString, QString> vals;

                    while(true) {
                        if (!showingLogin(QStringLiteral("4shared auth"), vals[QStringLiteral("login")], vals[QStringLiteral("password")], err))
                            return false;

                        QUrl form_url = form -> serializeFormToUrl(vals);
                        Html::Document doc = Manager::prepare() -> formFollowed(form_url) -> toHtml();
                        form = doc.findFirst("form[name='loginForm']");

                        if (!form) {
                            Html::Document acc_doc = Manager::prepare() -> getFollowed(url_html_user_root) -> toHtml();
                            Html::Tag * root_id_tag = acc_doc.findFirst("input.jsRootId");
                            if (root_id_tag)
                                user_id = root_id_tag -> value();
                            else
                                qDebug() << "Cannot find root id for current user";

                            return true;
                        }
                    }

                    return false;
                }
            };
        }
    }
}

#endif // FOURSHARED_AUTH
