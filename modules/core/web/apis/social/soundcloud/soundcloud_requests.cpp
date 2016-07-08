#include "soundcloud_requests.h"

using namespace Core::Web::Soundcloud;

void Requests::jsonToUsers(QList<Linkable> & linkables, const QJsonArray & arr) {
    if (arr.size() == 1 && arr[0].toObject().contains(QStringLiteral("errors"))) return;

    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
        QJsonObject obj = (*obj_iter).toObject();

        QString full_name = obj.value(tkn_full_name).toString();
        QString perma = obj.value(tkn_permalink).toString();

        linkables << Linkable(
            QString::number(obj.value(tkn_id).toInt()),
            full_name.isEmpty() ? obj.value(tkn_username).toString() : full_name,
            perma,
            obj.value(tkn_avatar_url).toString()
        );
    }
}

void Requests::jsonToGroups(QList<Linkable> & linkables, const QJsonArray & arr) {
    if (arr.size() == 1 && arr[0].toObject().contains(QStringLiteral("errors"))) return;

    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
        QJsonObject obj = (*obj_iter).toObject();

        linkables << Linkable(
            QString::number(obj.value(tkn_id).toInt()),
            obj.value(QStringLiteral("name")).toString(),
            obj.value(tkn_permalink).toString(),
            obj.value(tkn_artwork_url).toString()
        );
    }
}

bool Requests::takeOfflineCredentials() {
    if (hasOfflineCredentials()) return true;

    QString html = Manager::prepare() -> getFollowed(url_site_base) -> toText();
    QRegularExpression reg("sc_version = \"(\\d+)\"");
    QRegularExpressionMatch match = reg.match(html);

    if (match.hasMatch()) {
        QString app_id = match.captured(1);
        setSiteHash(app_id);

        QRegularExpression js_reg("(http[:\\/\\w\\.\\-]+app-[\\w-]+\\.js)");
        match = js_reg.match(html);
        if (match.hasMatch()) {
            QString js_url = match.captured(1);
            QString js_content = Manager::prepare() -> getFollowed(js_url) -> toText();

            QRegularExpression id("client_id:\"(\\w+)\"");
            match = id.match(js_content);
            if (match.hasMatch()) {
                QString token = match.captured(1);
                setSiteToken(token);
                return true;
            }
        }
    }

    return false;
}
