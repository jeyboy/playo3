#ifndef SOUNDCLOUD_API_PRIVATE_H
#define SOUNDCLOUD_API_PRIVATE_H

#include <QUrl>
#include <QUrlQuery>

class SoundcloudApiPrivate {
public:
    static inline QString clientId() { return "8f84790a84f5a5acd1c92e850b5a91b7"; }
    static inline int amountLimit() { return 200; }
    static inline int offsetLimit() { return 1000; /*8000*/; }

    static QString authUrl();
    static inline QString confirmAuthUrl(QString access_token) {
        return "https://api.soundcloud.com/me.json?oauth_token=" + access_token;
    }
    static QByteArray authTokenUrlParams(QString code);

    static void setAudioTypesParam(QUrlQuery & query);
    static void setAmountLimitation(QUrlQuery & query, int offset = 0);
    static void setSearchPredicate(QUrlQuery & query, QString & predicate);
    static void setGenreLimitation(QUrlQuery & query, QString & genre);
    static void setOrder(QUrlQuery & query, bool hottest);

    static QUrl audiosSearchUrl(QString predicate, QString genre, bool hottest = false, int offset = 0);

    static QUrl groupAudiosUrl(QString uid, int offset = 0);
    static QUrl groupPlaylistsUrl(QString uid, int offset = 0);

    static QUrl audioUrl(QString audio_uid);

    static QUrl userAudiosUrl(QString uid, int offset = 0);
    static QUrl userPlaylistsUrl(QString uid, int offset = 0);
    static QUrl userFolowingsUrl(QString uid, int offset = 0);
    static QUrl userFolowersUrl(QString uid, int offset = 0);
    static QUrl userGroupsUrl(QString uid, int offset = 0);

    static QUrlQuery commonMethodParams();
protected:
    inline static QString getApiUrl() { return "https://api.soundcloud.com/"; }
    static QUrlQuery userMethodParams(QString token);
};
#endif // SOUNDCLOUD_API_PRIVATE_H

