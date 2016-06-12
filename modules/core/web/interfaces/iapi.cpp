#include "iapi.h"

using namespace Core::Web;

bool IApi::showingCaptcha(const QUrl & pict_url, QString & result) {
    actionDialog -> buildCaptchaForm(Manager::prepare() -> pixmapGet(pict_url));
    bool res = actionDialog -> exec();
    result = actionDialog -> getValue(actionDialog -> captcha_key);
    return res;
}

bool IApi::showingLogin(const QString & title, QString & login, QString & pass, const QString & err) {
    actionDialog -> buildLoginForm(err, login, pass);
    actionDialog -> setWindowTitle(title);

    bool res = actionDialog -> exec();

    login = actionDialog -> getValue(actionDialog -> login_key);
    pass = actionDialog -> getValue(actionDialog -> pass_key);

    return res;
}

bool IApi::showingLoginWithCaptcha(const QString & title, const QUrl & pict_url, QString & login, QString & pass, QString & captcha, const QString & err) {
    actionDialog -> buildLoginWithCaptchaForm(Manager::prepare() -> pixmapGet(pict_url), err, login, pass);
    actionDialog -> setWindowTitle(title);

    bool res = actionDialog -> exec();

    login = actionDialog -> getValue(actionDialog -> login_key);
    pass = actionDialog -> getValue(actionDialog -> pass_key);
    captcha = actionDialog -> getValue(actionDialog -> captcha_key);

    return res;
}
