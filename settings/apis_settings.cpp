#include "apis_settings.h"

void ApisSettings::fromJson(const QJsonObject & json) {
    _ignoreCaptcha = json.value(SETTINGS_IGNORE_CAPTCHA_KEY).toBool(false);
}

void ApisSettings::toJson(QJsonObject & json) {
    json.insert(SETTINGS_IGNORE_CAPTCHA_KEY, _ignoreCaptcha);
}
