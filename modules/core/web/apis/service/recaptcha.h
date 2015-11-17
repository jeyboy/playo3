#ifndef RECAPTCHA
#define RECAPTCHA

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/utils/web_manager.h"

#define RECAPTCHA_BASE_URL QStringLiteral("http://www.google.com/recaptcha/api/")
#define RECAPTCHA_CHALLENGE_URL RECAPTCHA_BASE_URL % QStringLiteral("challenge?k=")
#define RECAPTCHA_RELOAD_URL RECAPTCHA_BASE_URL % QStringLiteral("reload?c=")

namespace Core {
    namespace Web {
        namespace Recaptcha {
            class V1: public Singleton<V1> {
                friend class Singleton<V1>;
                V1() {}

                QUrl buildReloadUrl(const QString & key, const QString & challenge, const QString & lang, const QString & content_type) {
                    return QUrl(
                        RECAPTCHA_BASE_URL % QStringLiteral("reload?c=") % challenge
                        % QStringLiteral("&k=") % key
                        % QStringLiteral("&reason=i")
                        % QStringLiteral("&type=") % content_type
                        % QStringLiteral("&lang=") % lang
                        // th param is not calculable at this time :(
                    );
                }
                QUrl buildChallengeUrl(const QString & key) { return QUrl(RECAPTCHA_CHALLENGE_URL % key); }
                QUrl buildPictUrl(const QString & challenge) { return QUrl(RECAPTCHA_BASE_URL % QStringLiteral("image?c=") % challenge); }
            public:
                //                Recaptcha.finish_reload('03AHJ_VuvoR-17M1qLHt_V9U0CWwIFxUbXOiOngRNMOjmT_yKcWu0HdsAkWidBT213JAM_gdbovYJukKmgxxMc9mlpmHqedOXqs4WGXri4eOWy4u0MaHFO6s3Pua8V6KMcapFLZj8Ri-vFXRbR-4tJgsNHfKUhmyzDkEtTKZabSV1hlupV2Nq5Jp88fGbdXOjK5vIf_BmguQJvcTb-oDLVshQDeBRPXKStYZEmLX2JzgZMP4OJy_KWmyH4XES0CEq9pJucFHwKovkzrd36-DOm1eiw0jtt5EDgTQ', 'image', null, null);
                QUrl takeNewImageUrl(const QString & key, QString & challenge, const QString & lang = QStringLiteral("en"), const QString & content_type = QStringLiteral("image")) {
                    QUrl rel_url = buildReloadUrl(key, challenge, lang, content_type);
                    qDebug() << "REL URL" << rel_url;
                    QString response = Manager::prepare() -> followedGet(rel_url) -> toText();
                    challenge = response.section(QStringLiteral("finish_reload('"), -1).section('\'', 0, 0);
                    qDebug() << "RECAPTCHA" << response;
                    qDebug() << "RECAPTCHA STR" << challenge;
                    return buildPictUrl(challenge);
                }

                QUrl takeImageUrl(const QString & key, QString & challenge) {
                    QString key_str = key.section(QStringLiteral("?k="), -1);
                    QUrl challenge_url = buildChallengeUrl(key_str);
                    QString challenge_manifest = Manager::prepare() -> followedGet(challenge_url) -> toText();
                    challenge = challenge_manifest.section(QStringLiteral("challenge : '"), -1).section('\'', 0, 0);
                    return takeNewImageUrl(key_str, challenge);
//                    return buildPictUrl(challenge);
                }
            };
        }
    }
}

#endif // RECAPTCHA
