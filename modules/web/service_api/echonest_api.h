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
    inline QString name() const { return QStringLiteral("echonest"); }
    inline Playo3::WebSubType siteType() { return Playo3::echonest_site; }
    virtual ~EchonestApi() { }

    static EchonestApi * instance();
    inline static void close() { delete self; }
protected:
    inline Json::Arr search_postprocess(QString & /*predicate*/, QString & /*genre*/, const SearchLimit & /*limitations*/) { return Json::Arr();}

    inline QString refresh(QString path) { return path; }
    inline QUrlQuery genDefaultParams() { return QUrlQuery(QStringLiteral("api_key=TSCA6XDZTJQ1OOJSV")); }

    inline QString baseUrlStr(const QString & predicate) { return QStringLiteral("http://developer.echonest.com/api/v4/") % predicate; }

    inline QString offsetKey() const { return QStringLiteral("start"); }
    inline QString limitKey() const { return QStringLiteral("results"); }
    inline int requestLimit() const { return 100; }

    inline Json::Obj & extractBody(Json::Obj & response) { return (response = response.obj(QStringLiteral("response"))); }
    inline bool endReached(Json::Obj & response, int offset) { return offset >= extractBody(response).value(QStringLiteral("total")).toInt(); }
    inline bool extractStatus(QUrl & /*url*/, Json::Obj & response, int & code, QString & message) {
        Json::Obj stat_obj = extractBody(response).obj(QStringLiteral("status"));
        message = stat_obj.str(QStringLiteral("message"));
        return (code = stat_obj.intVal(QStringLiteral("code"))) == 0;
    }
private:
    inline EchonestApi() : QObject() { }

    static EchonestApi * self;
};

#endif // ECHONEST_API_H
