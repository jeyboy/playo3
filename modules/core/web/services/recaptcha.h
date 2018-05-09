#ifndef RECAPTCHA
#define RECAPTCHA

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/utils/web_manager.h"

#define RECAPTCHA_BASE_URL QLatin1String("https://www.google.com/recaptcha/api/")
#define RECAPTCHA_CHALLENGE_URL RECAPTCHA_BASE_URL % QLatin1String("challenge?k=")
#define RECAPTCHA_RELOAD_URL RECAPTCHA_BASE_URL % QLatin1String("reload?c=")

namespace Core {
    namespace Web {
        namespace Recaptcha {
            class V1: public Singleton<V1> {
                friend class Singleton<V1>;
                V1() {}

                QUrl buildReloadUrl(const QString & key, const QString & challenge, const QString & lang, const QString & content_type) {
                    return QUrl(
                        RECAPTCHA_BASE_URL % QLatin1String("reload?c=") % challenge
                        % QLatin1String("&k=") % key
                        % QLatin1String("&reason=i")
                        % QLatin1String("&type=") % content_type
                        % QLatin1String("&lang=") % lang
                        // th param is not calculable at this time :(
                    );
                }
                QUrl buildChallengeUrl(const QString & key) { return QUrl(RECAPTCHA_CHALLENGE_URL % key); }
                QUrl buildPictUrl(const QString & challenge) { return QUrl(RECAPTCHA_BASE_URL % QLatin1String("image?c=") % challenge); }
            public:
                QUrl takeNewImageUrl(const QString & key, QString & challenge, const QString & lang = QLatin1String("en"), const QString & content_type = QStringLiteral("image")) {
                    QUrl rel_url = buildReloadUrl(key, challenge, lang, content_type);
                    QString response = Manager::prepare() -> getFollowed(rel_url) -> toText();
                    challenge = response.section(QLatin1String("finish_reload('"), -1).section('\'', 0, 0);
                    return buildPictUrl(challenge);
                }

                QUrl takeImageUrl(const QString & key, QString & challenge) {
                    QString key_str = key.section(QLatin1String("?k="), -1);
                    QUrl challenge_url = buildChallengeUrl(key_str);
                    QString challenge_manifest = Manager::prepare() -> getFollowed(challenge_url) -> toText();
                    challenge = challenge_manifest.section(QLatin1String("challenge : '"), -1).section('\'', 0, 0);
                    return takeNewImageUrl(key_str, challenge);
//                    return buildPictUrl(challenge);
                }
            };
        }
    }
}

#endif // RECAPTCHA
