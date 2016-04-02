#ifndef APIS_SETTINGS
#define APIS_SETTINGS

#include <qvariant.h>
#include <qjsonobject.h>

#define SETTINGS_IGNORE_CAPTCHA_KEY QStringLiteral("ignore_captcha")

class ApisSettings {
public:
    void fromJson(const QJsonObject & json);
    void toJson(QJsonObject & json);

    inline bool isIgnoreCaptcha() const { return _ignoreCaptcha; }
    inline void setIgnoreCaptcha(bool ignore) { _ignoreCaptcha = ignore; }
protected:
    bool _ignoreCaptcha;
};

#endif // APIS_SETTINGS
