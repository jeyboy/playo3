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

QUrl SoundcloudApiPrivate::groupAudiosUrl(QString uid) {
    QUrlQuery query = commonMethodParams();
    setAudioTypesParam(query);

    QUrl url(getApiUrl() + "groups/" + uid + "/tracks.json");
    url.setQuery(query);
    return url;
}

QUrl SoundcloudApiPrivate::groupPlaylistsUrl(QString uid) {
    QUrl url(getApiUrl() + "groups/" + uid + "/playlists.json");
    url.setQuery(commonMethodParams());
    return url;
}

QUrl SoundcloudApiPrivate::userAudiosUrl(QString uid) {
    QUrlQuery query = commonMethodParams();
    setAudioTypesParam(query);

    QUrl url(getApiUrl() + "users/" + uid + "/tracks.json");
    url.setQuery(query);
    return url;
}

QUrl SoundcloudApiPrivate::userPlaylistsUrl(QString uid) {
    QUrl url(getApiUrl() + "users/" + uid + "/playlists.json");
    url.setQuery(commonMethodParams());
    return url;
}

QUrl SoundcloudApiPrivate::userFolowingsUrl(QString uid) {
    QUrl url(getApiUrl() + "users/" + uid + "/followings.json");
    url.setQuery(commonMethodParams());
    return url;
}

QUrl SoundcloudApiPrivate::userFolowersUrl(QString uid) {
    QUrl url(getApiUrl() + "users/" + uid + "/followers.json");
    url.setQuery(commonMethodParams());
    return url;
}

QUrl SoundcloudApiPrivate::userGroupsUrl(QString uid) {
    QUrl url(getApiUrl() + "users/" + uid + "/groups.json");
    url.setQuery(commonMethodParams());
    return url;
}
