#include "od_api.h"

using namespace Core::Web::Od;

QString Api::refresh(const QString & refresh_page, const DataMediaType & /*itemMediaType*/) { // here refresh_page must by eq to track id
    QJsonObject obj = Manager::prepare() -> jsonGet(playAudioUrl(refresh_page));
    if (hasError(obj)) {
        connectUser();
        obj = Manager::prepare() -> jsonGet(playAudioUrl(refresh_page));
        qDebug() << "RECONECTION";
    }
    QUrl url(obj.value(tkn_play).toString());
    QUrlQuery query = QUrlQuery(url.query());
    query.addQueryItem(tkn_client_hash, calcMagicNumber(query.queryItemValue(tkn_md5)));
    url.setQuery(query);
    return url.toString();
}
