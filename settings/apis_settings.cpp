#include "apis_settings.h"

void ApisSettings::fromJson(const QJsonObject & json) {
    _ignore_captcha = json.value(SETTINGS_IGNORE_CAPTCHA_KEY).toBool(false);
    _use_ignore_list = json.value(SETTINGS_USE_IGNORE_LIST_KEY).toBool(false);
}

void ApisSettings::toJson(QJsonObject & json) {
    json.insert(SETTINGS_IGNORE_CAPTCHA_KEY, _ignore_captcha);
    json.insert(SETTINGS_USE_IGNORE_LIST_KEY, _use_ignore_list);
}
