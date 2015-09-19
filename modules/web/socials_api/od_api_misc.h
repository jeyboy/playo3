#ifndef OD_API_MISC
#define OD_API_MISC

#include <qstring.h>
#include <qcryptographichash.h>

#include "../iapi.h"
#include "../web_api.h"
#include "misc/web_utils/html_parser.h"
#include "od_api_keys.h"

namespace Od {
    class Misc : public WebApi, public IApi {
            int magic [33] = { 4, 3, 5, 6, 1, 2, 8, 7, 2, 9, 3, 5, 7, 1, 4, 8, 8, 3, 4, 3, 1, 7, 3, 5, 9, 8, 1, 4, 3, 7, 2, 8 };
        protected:
            QString authE, authP;

            inline Misc(QObject * parent = 0) : WebApi(parent) {}

            inline QUrl authSidUrl() { return QUrl(baseUrlStr(QStringLiteral("web-api/music/conf"))); }

            inline void nullifyCredentials() {
                authE.clear();
                authP.clear();
            }

            inline bool checkCredentials() {
                if (authE.isEmpty() || authP.isEmpty())
                    return showingLogin(authE, authP);

                return true;
            }

            inline bool hasError(const QJsonObject & obj) {
                return obj.contains(QStringLiteral("error"));
            }

            inline QHash<QString, QString> initHeaders() {
                QHash<QString, QString> headers;
    //            headers.insert(QStringLiteral("User-Agent"), QStringLiteral("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0"));
                headers.insert(QStringLiteral("User-Agent"), QStringLiteral("Mozilla/5.0 (Windows NT 6.1; WOW64; rv:39.0) Gecko/20100101 Firefox/39.0"));
                return headers;
            }

            inline QString grabUserId(Html::Document & doc) {
                Html::Set results = doc.find("a.u-menu_a.tdn[href^'/profile']");
                if (results.isEmpty()) {
                    Html::Set results = doc.find(".ff_links_li a[href~'st.uid=']]");
                    return results.link().section("st.uid=", 1).section('&', 0, 0);
                }
                else return results.link().section('/', 2).section('?', 0, 0);

                return QString();
            }

            inline QString grabSID() {
                QJsonObject obj = WebManager::manager() -> postJson(authSidUrl(), initHeaders()); // calculate sid for requests
                if (obj.contains(QStringLiteral("sid")))
                    return obj.value(QStringLiteral("sid")).toString();
                else {
                    qDebug() << "SID ERROR" << obj;
                    return QString();
                }
            }

            inline void checkSecurity(Html::Document & doc) {
                Html::Set forms = doc.find("[id^'hook_Form'] form");
    //            doc.output();

                if (!forms.isEmpty()) {
                    QList<FormInput> inputs;
                    inputs << FormInput(QStringLiteral("code"), true, QStringLiteral("Code from sms"));
                    inputs << FormInput(QStringLiteral("Resend sms"), forms.find("#accRcvrSent").link(), WebManager::manager(), "sendGet");
                    actionDialog -> buildForm(inputs);

                    if (actionDialog -> exec() == QDialog::Accepted) {
                        QHash<QString, QString> attrs;
                        attrs.insert("st.mobileCaptcha", actionDialog -> getValue(QStringLiteral("code")));
                        QUrl url = QUrl(base_url).resolved(forms.first() -> toFormSubmit(attrs));
                        QNetworkReply * reply = WebManager::manager() -> followedForm(url, initHeaders());
                        //TODO: check session
                        reply -> deleteLater();
                    }
                }
            }

            QString calcMagicNumber(const QString & md5) {
                QCryptographicHash hash(QCryptographicHash::Md5);
                hash.addData((md5 % QStringLiteral("secret")).toLatin1());
                QByteArray src = hash.result().toHex().toLower();

                QList<int> newSrc; newSrc.reserve(src.size() + 1);
                for(QByteArray::Iterator chr = src.begin(); chr != src.end(); chr++)
                    newSrc << ((*chr) - ((*chr) < 58 ? 48 : 87));

                int sum = 0;
                for(QList<int>::Iterator num = newSrc.begin(); num != newSrc.end(); num++)
                    sum += (*num);

                newSrc << newSrc.last();

                QString res;
                for (int x = 0; x < 32; x++)
                    res.append(QString::number(qAbs(sum - newSrc[x + 1] * newSrc[x] * magic[x])));

                return res;
            }
    };
}

#endif // OD_API_MISC
