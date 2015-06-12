#ifndef ECHONEST_API_H
#define ECHONEST_API_H

//Currently supported ID spaces are:
//Deezer artists - Example: deezer:artist:399
//Deezer tracks - Example: deezer:track:20592361
//Free Music Archive artists - Example: fma:artist:3243
//Free Music Archive tracks - Example: fma:track:11764
//Spotify artists - Example: spotify:artist:4Z8W4fKeB5YxbusRsdQVPb or spotify-XX:artist:4Z8W4fKeB5YxbusRsdQVPb where XX is the locale. Supported locales are AD AR AT AU BE BG BO BR CA CH CL CO CR CY CZ DE DK DO EC EE ES FI FR GB GR GT HK HN HR HU IE IS IT LI LT LU LV MC MT MX MY NI NL NO NZ PA PE PH PL PT PY RO SE SG SI SK SV TR TW US UY ZZ
//Spotify tracks - Example: spotify:track:3L7BcXHCG8uT92viO6Tikl or spotify-XX:track:3L7BcXHCG8uT92viO6Tikl where XX is the locale. Supported locales are AD AR AT AU BE BG BO BR CA CH CL CO CR CY CZ DE DK DO EC EE ES FI FR GB GR GT HK HN HR HU IE IS IT LI LT LU LV MC MT MX MY NI NL NO NZ PA PE PH PL PT PY RO SE SG SI SK SV TR TW US UY ZZ

//lyrics
//MusixMatch songs - Example: musixmatch-WW:song:3310380
//SongMeanings artists - Example: songmeaningsg:artist:200
//SongMeanings songs - Example: songmeanings:song:471679

#include "echonest_genre_api.h"
#include "echonest_artist_api.h"
#include "echonest_song_api.h"
#include "echonest_playlist_api.h"
#include "misc/web_utils/custom_network_access_manager.h"

class EchonestApi : public QObject, public EchonestGenreApi, public EchonestArtistApi,
        public EchonestSongApi, public EchonestPlaylistApi {
    Q_OBJECT
public:
    inline QString name() const { return "echonest"; }
    ~EchonestApi() { }

    static EchonestApi * instance();
    inline static void close() { delete self; }
protected:
    inline QString baseUrl(QString predicate) { return "http://developer.echonest.com/api/v4/" + predicate; }

    inline QString token() const { return "TSCA6XDZTJQ1OOJSV"; }
    inline int requestLimit() { return 100; }

    inline int extractAmount(QJsonObject & response) {
        return response.value("response").toObject().value("total").toInt();
    }
    inline void setLimit(QUrlQuery & query, int limit, int offset = 0) {
        if (offset > 0) EchonestGenreApi::setParam(query, "start", QString::number(offset));
        EchonestGenreApi::setParam(query, "results", QString::number(limit));
    }

    bool proceedQuery(QUrl url, QJsonObject & response) {
        CustomNetworkAccessManager * manager;
        bool isNew = CustomNetworkAccessManager::validManager(manager);

        response = manager -> getToJson(QNetworkRequest(url));

        if (isNew) delete manager;
        bool status = response.value("response").toObject().value("status").toObject().value("code").toInt() == 0;

        if (!status)
            qDebug() << response.value("response").toObject().value("status").toObject().value("message").toString();

        return status;
    }

protected slots:
    void requestFinished() {
//        QFutureWatcher<ApiFuncContainer *> * initiator = (QFutureWatcher<ApiFuncContainer *> *)sender();
//        ApiFuncContainer * func = initiator -> result();
//        connect(this, SIGNAL(routineFinished(QJsonObject &)), func -> obj, func -> slot);
//        emit routineFinished(func -> result);
//        disconnect(this, SIGNAL(routineFinished(QJsonObject &)), func -> obj, func -> slot);
//        delete func;
    }

private:
    inline EchonestApi() : QObject() { }

    static EchonestApi * self;
};

#endif // ECHONEST_API_H
