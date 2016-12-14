#ifndef APIS_SETTINGS
#define APIS_SETTINGS

#include <qvariant.h>
#include <qjsonobject.h>

#define SETTINGS_IGNORE_CAPTCHA_KEY QStringLiteral("ignore_captcha")
#define SETTINGS_USE_IGNORE_LIST_KEY QStringLiteral("use_ignore_list")

class ApisSettings {
public:
    void fromJson(const QJsonObject & json);
    void toJson(QJsonObject & json);

    inline bool isIgnoreCaptcha() const { return _ignore_captcha; }
    inline void setIgnoreCaptcha(bool ignore) { _ignore_captcha = ignore; }

    inline bool isUseIgnoreList() const { return _use_ignore_list; }
    inline void useIgnoreList(bool use) { _use_ignore_list = use; }
protected:
    bool _ignore_captcha;
    bool _use_ignore_list;
};

#endif // APIS_SETTINGS
