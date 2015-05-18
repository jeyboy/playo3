#include "soundcloud_api_private.h"

QUrlQuery SoundcloudApiPrivate::userMethodParams(QString token) {
    QUrlQuery query = QUrlQuery();
    query.addQueryItem("oauth_token", token); //getToken()
    return query;
}

QUrlQuery SoundcloudApiPrivate::commonMethodParams() {
    QUrlQuery query = QUrlQuery();
    query.addQueryItem("client_id", clientId());
    return query;
}

QString SoundcloudApiPrivate::authUrl() {
    QUrl url("https://soundcloud.com/connect");

    QUrlQuery queryParams = QUrlQuery();
    queryParams.addQueryItem("client_id", clientId());
    queryParams.addQueryItem("response_type", "code");
    queryParams.addQueryItem("scope", "non-expiring");
    queryParams.addQueryItem("redirect_uri", "http://sos.com");
    queryParams.addQueryItem("display", "popup");

    url.setQuery(queryParams);
    return url.toString();
}

QByteArray SoundcloudApiPrivate::authTokenUrlParams(QString code) {
    QUrlQuery queryParams = QUrlQuery();

    queryParams.addQueryItem("client_id", clientId());
    queryParams.addQueryItem("client_secret", "54ca588303e1d2bf524509faf20931b4");
    queryParams.addQueryItem("grant_type", "authorization_code");
    queryParams.addQueryItem("redirect_uri", "http://sos.com");
    queryParams.addQueryItem("code", code);

    return queryParams.toString(QUrl::FullyEncoded).toUtf8();
}

//QString SoundcloudApiPrivate::authTokenUrl() const {
//    QUrl url("https://api.soundcloud.com/oauth2/token");
//    QUrlQuery queryParams = QUrlQuery();


//    queryParams.addQueryItem("client_id", "8f84790a84f5a5acd1c92e850b5a91b7");
//    queryParams.addQueryItem("client_secret", "54ca588303e1d2bf524509faf20931b4");
//    queryParams.addQueryItem("scope", "non-expiring");

////    queryParams.addQueryItem("grant_type", "password");
////    queryParams.addQueryItem("username", "USERNAME");
////    queryParams.addQueryItem("password", "PASSWORD");

//    url.setQuery(queryParams);
//    return url.toString();
//}

void SoundcloudApiPrivate::setAudioTypesParam(QUrlQuery & query) {
    query.addQueryItem("types", "original,remix,live,podcast");
    //    “original”
    //    “remix”
    //    “live”
    //    “recording”
    //    “spoken”
    //    “podcast”
    //    “demo”
    //    “in progress”
    //    “stem”
    //    “loop”
    //    “sound effect”
    //    “sample”
    //    “other”
}

void SoundcloudApiPrivate::setAmountLimitation(QUrlQuery & query, int offset) {
    query.addQueryItem("limit", QString::number(amountLimit()));
    query.addQueryItem("offset", QString::number(offset));
}

void SoundcloudApiPrivate::setSearchPredicate(QUrlQuery & query, QString & predicate) {
    query.addQueryItem("q", predicate);
}

void SoundcloudApiPrivate::setGenreLimitation(QUrlQuery & query, QString & genre) {
    query.addQueryItem("genres", genre);
}

QUrl SoundcloudApiPrivate::audiosSearchUrl(QString predicate, QString genre, int offset) {
    QUrlQuery query = commonMethodParams();
    setAudioTypesParam(query);
    setAmountLimitation(query, offset);

    if (!genre.isEmpty())
        setGenreLimitation(query, genre);

    if (!predicate.isEmpty())
        setSearchPredicate(query, predicate);

    QUrl url(getApiUrl() + "tracks.json");
    url.setQuery(query);
    return url;
}


QUrl SoundcloudApiPrivate::groupAudiosUrl(QString uid, int offset) {
    QUrlQuery query = commonMethodParams();
    setAudioTypesParam(query);
    setAmountLimitation(query, offset);

    QUrl url(getApiUrl() + "groups/" + uid + "/tracks.json");
    url.setQuery(query);
    return url;
}

QUrl SoundcloudApiPrivate::groupPlaylistsUrl(QString uid, int offset) {
    QUrl url(getApiUrl() + "groups/" + uid + "/playlists.json");
    QUrlQuery query = commonMethodParams();
    setAmountLimitation(query, offset);
    url.setQuery(query);

    return url;
}

QUrl SoundcloudApiPrivate::userAudiosUrl(QString uid, int offset) {
    QUrlQuery query = commonMethodParams();
    setAudioTypesParam(query);
    setAmountLimitation(query, offset);

    QUrl url(getApiUrl() + "users/" + uid + "/tracks.json");
    url.setQuery(query);
    return url;
}

QUrl SoundcloudApiPrivate::userPlaylistsUrl(QString uid, int offset) {
    QUrl url(getApiUrl() + "users/" + uid + "/playlists.json");
    QUrlQuery query = commonMethodParams();
    setAmountLimitation(query, offset);
    url.setQuery(query);

    return url;
}

QUrl SoundcloudApiPrivate::userFolowingsUrl(QString uid, int offset) {
    QUrl url(getApiUrl() + "users/" + uid + "/followings.json");
    QUrlQuery query = commonMethodParams();
    setAmountLimitation(query, offset);
    url.setQuery(query);

    return url;
}

QUrl SoundcloudApiPrivate::userFolowersUrl(QString uid, int offset) {
    QUrl url(getApiUrl() + "users/" + uid + "/followers.json");
    QUrlQuery query = commonMethodParams();
    setAmountLimitation(query, offset);
    url.setQuery(query);

    return url;
}

QUrl SoundcloudApiPrivate::userGroupsUrl(QString uid, int offset) {
    QUrl url(getApiUrl() + "users/" + uid + "/groups.json");
    QUrlQuery query = commonMethodParams();
    setAmountLimitation(query, offset);
    url.setQuery(query);

    return url;
}
