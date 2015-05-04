#ifndef SOUNDCLOUD_API_PRIVATE_H
#define SOUNDCLOUD_API_PRIVATE_H

#include <QUrl>
#include <QUrlQuery>

class SoundcloudApiPrivate {
public:
    static inline QString clientId() { return "8f84790a84f5a5acd1c92e850b5a91b7"; }

    static QString authUrl();
    static inline QString confirmAuthUrl(QString access_token) {
        return "https://api.soundcloud.com/me.json?oauth_token=" + access_token;
    }
    static QByteArray authTokenUrlParams(QString code);

    static void setAudioTypesParam(QUrlQuery & query);

    static QUrl groupAudiosUrl(QString uid);
    static QUrl groupPlaylistsUrl(QString uid);

    static QUrl userAudiosUrl(QString uid);
    static QUrl userPlaylistsUrl(QString uid);
    static QUrl userFolowingsUrl(QString uid);
    static QUrl userFolowersUrl(QString uid);
    static QUrl userGroupsUrl(QString uid);

    static QUrlQuery commonMethodParams();
protected:
    inline static QString getApiUrl() { return "https://api.soundcloud.com/"; }
    static QUrlQuery userMethodParams(QString token);
};
#endif // SOUNDCLOUD_API_PRIVATE_H

