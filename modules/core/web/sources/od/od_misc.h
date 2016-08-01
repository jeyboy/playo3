#ifndef OD_API_MISC
#define OD_API_MISC

#include <qstring.h>
#include <qcryptographichash.h>

#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/iuser_interaction.h"
#include "od_keys.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Misc : public IQueriable, public IUserInteraction {
                    int magic [33] = { 4, 3, 5, 6, 1, 2, 8, 7, 2, 9, 3, 5, 7, 1, 4, 8, 8, 3, 4, 3, 1, 7, 3, 5, 9, 8, 1, 4, 3, 7, 2, 8 };
                protected:
                    inline Misc() {}

                    virtual bool takeOnlineCredentials() = 0;

                    inline QUrl authSidUrl() { return QUrl(baseUrlStr(qst_site_def, path_sid)); }

                    inline bool hasError(const QJsonObject & obj) { return obj.contains(tkn_error); }

                    inline bool retryRequired(const QJsonObject & obj, QString & message) {
                        message = obj.value(tkn_error).toString();
                        return message == QStringLiteral("error.notloggedin");
                    }

                    inline Headers initHeaders() { return {{tkn_header_user_agent, DEFAULT_AGENT}}; }

                    inline QString grabUserId(Html::Document & doc) {
                        Html::Set results = doc.find("a.u-menu_a.tdn[href^'/profile']");
                        if (results.isEmpty()) {
                            Html::Set results = doc.find(".ff_links_li a[href~'st.uid=']]");
                            return results.link().section("st.uid=", 1).section('&', 0, 0);
                        }
                        else return results.link().section('/', 2).section('?', 0, 0);
                    }

                    inline QString grabSID() {
                        QJsonObject obj = Manager::prepare() -> jsonPost(authSidUrl(), initHeaders()); // calculate sid for requests
                        if (obj.contains(tkn_sid))
                            return obj.value(tkn_sid).toString();
                        else {
                            qDebug() << "SID ERROR" << obj;
                            return QString();
                        }
                    }

                    inline void checkSecurity(Html::Document & doc) {
                        doc.output();
                        Html::Tag * form = doc.findFirst("[id^'hook_Form'] form");

                        if (!form) {
                            QList<FormInput> inputs;
                            inputs << FormInput::createStr(tkn_code, val_sms_code_title);
                            inputs << FormInput(val_resend_sms_title, doc.find("#accRcvrSent").link(), Manager::prepare(), "sendGet");
                            actionDialog -> proceedForm(inputs);

                            if (actionDialog -> exec() == QDialog::Accepted) {
                                QHash<QString, QString> attrs = {{tkn_captcha, actionDialog -> getValue(tkn_code)}};
                                QUrl url = QUrl(url_root).resolved(form -> serializeFormToUrl(attrs));
                                QNetworkReply * reply = Manager::prepare() -> formFollowed(url, initHeaders());
                                //TODO: check session
                                reply -> deleteLater();
                            }
                        }
                    }

                    QString calcMagicNumber(const QString & md5) {
                        QCryptographicHash hash(QCryptographicHash::Md5);
                        hash.addData((md5 % tkn_secret).toLatin1());
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
    }
}

#endif // OD_API_MISC
