#ifndef VK_API_PRIVATE_H
#define VK_API_PRIVATE_H

#include <QUrl>
#include <QUrlQuery>
#include <QStringList>

class VkApiPrivate {
public:
    static QString authUrl();
    static QUrl wallUrl(QString uid, QString token, int offset, int count);
    static QUrl audioRefreshUrl(QStringList uids, QString token);
    static QUrl audioAlbumsUrl(QString uid, QString token, int offset);
    static QUrl audioInfoUrl(QString uid, QString currUid, QString token);
    static QUrl audioRecomendationUrl(QString uid, bool byUser, bool randomize, QString token);
    static QUrl audioPopularUrl(bool onlyEng, QString token, int genreId = -1);
    static QUrl audioSearchUrl(QString searchStr, bool autoFix, bool artistOnly, bool searchByOwn, int sort, QString token);
    static QUrl audioLyricsUrl(QString token, QString lyrics_id);

    static QUrl isAppUser(QString token, QString uid);

protected:
    inline static QString boolToStr(bool val) { return val ? "1" : "0"; }
    inline static QString apiVersion() { return "5.21"; }
    static QUrlQuery methodParams(QString & token);
    inline static QString getApiUrl() { return "https://api.vk.com/method/"; }
    inline static QString getObjLimit() { return "20"; }

//    QUrl getAudioListUrl() const;
//    QUrl getAudioCountUrl() const;
//    QUrl getAudioSearchUrl() const;
//    QUrl getAudioCopyUrl() const;
//    QUrl getAudioRemoveUrl() const;

//    QUrl getAudioAlbumsListUrl() const;
//    QUrl getAudioAlbumAddUrl() const;
//    QUrl getAudioAlbumEditUrl() const;
//    QUrl getAudioAlbumRemoveUrl() const;
//    QUrl getAudioAlbumMoveToUrl() const;

//    QUrl getAudioSaveServerUrl() const;
//    QUrl getAudioSaveUrl() const;
};

#endif // VK_API_PRIVATE_H
