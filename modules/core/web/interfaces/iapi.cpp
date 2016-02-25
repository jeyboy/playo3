#include "iapi.h"

using namespace Core;

bool IApi::showingCaptcha(const QUrl & pict_url, QString & result) {
    actionDialog -> buildCaptchaForm(Web::Manager::prepare() -> getImage(pict_url));
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
    actionDialog -> buildLoginWithCaptchaForm(Web::Manager::prepare() -> getImage(pict_url), err, login, pass);
    actionDialog -> setWindowTitle(title);

    bool res = actionDialog -> exec();

    login = actionDialog -> getValue(actionDialog -> login_key);
    pass = actionDialog -> getValue(actionDialog -> pass_key);
    captcha = actionDialog -> getValue(actionDialog -> captcha_key);

    return res;
}

bool IApi::sQuery(QUrl url, QJsonObject & response, JsonPostProc post_proc, QObject * errorReceiver) {
    int code;
    QString message;
    Logger::obj().write(QStringLiteral("sQuery"), url.toString(), QStringLiteral("sending"));
    QUrl respUrl;
    response = Web::Manager::prepare() -> getJson(url, respUrl, post_proc & wrap);

    bool status = extractStatus(url, respUrl, response, code, message);
    if (!status) {
        Logger::obj().write(QStringLiteral("sQuery"), url.toString(), message, true);
        sendError(errorReceiver, message, code);
    } else {
        if (post_proc & extract) extractBody(response);
        Logger::obj().write(QStringLiteral("sQuery"), url.toString(), response.keys());
    }
    return status;
}

QJsonArray & IApi::lQuery(QUrl url, QueryRules rules, QJsonArray & result, JsonPostProc post_proc, QObject * errorReceiver) {
    QJsonObject response;

    while (sQuery(buildUrl(url, rules.offset, rules.limit, response), response, post_proc, errorReceiver)) {
        QJsonValue val = rules.field.isEmpty() ? QJsonValue(response) : response.value(rules.field);
        bool invalid = val.isArray();

        if (invalid) {
            QJsonArray ar = val.toArray();
            invalid = ar.isEmpty();
            rules.fact_count += ar.size();
        }

        if (!invalid)
            concatJsonArrays(result, val.toArray());

        iterateOffset(rules.offset, response, url);
        if (rules.offset >= rules.count || endReached(response, rules.offset)) break;
        else QThread::msleep(REQUEST_DELAY);
    }

    return result;
}
