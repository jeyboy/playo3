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

class EchonestApi : public QObject, public EchonestGenreApi, public EchonestArtistApi,
        public EchonestSongApi, public EchonestPlaylistApi {
    Q_OBJECT
public:
    inline QString name() const { return "echonest"; }
    ~EchonestApi() { }

    static EchonestApi * instance();
    inline static void close() { delete self; }
protected:
    inline QUrlQuery genDefaultParams() { return QUrlQuery("api_key=TSCA6XDZTJQ1OOJSV"); }

    inline QString baseUrlStr(QString & predicate) { return "http://developer.echonest.com/api/v4/" + predicate; }

    inline QString offsetKey() const { return "start"; }
    inline QString limitKey() const { return "results"; }
    inline int requestLimit() const { return 100; }

    inline QJsonObject & extractBody(QJsonObject & response) { return (response = response.value("response").toObject()); }
    inline bool endReched(QJsonObject & response, int offset) { return offset >= extractBody(response).value("total").toInt(); }
    inline void extractStatus(QJsonObject & response, int & code, QString & message) {
        QJsonObject stat_obj = extractBody(response).value("status").toObject();
        code = stat_obj.value("code").toInt();
        message = stat_obj.value("message").toString();
    }
protected slots:
//    void requestFinished() {
////        QFutureWatcher<ApiFuncContainer *> * initiator = (QFutureWatcher<ApiFuncContainer *> *)sender();
////        ApiFuncContainer * func = initiator -> result();
////        connect(this, SIGNAL(routineFinished(QJsonObject &)), func -> obj, func -> slot);
////        emit routineFinished(func -> result);
////        disconnect(this, SIGNAL(routineFinished(QJsonObject &)), func -> obj, func -> slot);
////        delete func;
//    }

private:
    inline EchonestApi() : QObject() { }

    static EchonestApi * self;
};

#endif // ECHONEST_API_H
