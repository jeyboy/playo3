#ifndef MUSICSHARA
#define MUSICSHARA

#include "modules/core/interfaces/isearchable.h"
#include "modules/core/interfaces/singleton.h"

// store all selectors in global variables
namespace Core {
    namespace Web {
        class MusicShara : public ISearchable, public Singleton<MusicShara> {
        public:
            inline QString name() const { return QStringLiteral("MusicShara"); }
            inline DataSubType siteType() const { return dt_site_music_shara; }

            // artists by genre
            QJsonArray byGenre(QString /*genre*/, const SearchLimit & /*limitations*/) { // https://myzuka.org/Genre/92/8-Bit https://myzuka.org/Genre/11/Pop/Page2
                QJsonArray json;
    //            if (genresList().isEmpty()) genresList();

    //            genre_id = genres.toInt(genre);
    //            if (genre_id == genres.defaultInt()) return json;

    //            WebManager * manager = WebManager::manager();
    //            QString genrePath = baseUrlStr(QStringLiteral("/Genre/%1/%2/Page").arg(QString::number(genre_id), genre));
    //            QHash<QString, QString> artistLinks;

    //            for(int page = 1; page < MAX_PAGE; page++) {
    //                QUrl url(genrePath % QString::number(page));
    //                QNetworkReply * response = manager -> getSync(url);
    //                Html::Document doc(response);

    //                doc.find(&searchTablesSelector).findLinks(&artistSelector, artistLinks);

    //                QThread::msleep(REQUEST_DELAY); // extra pause
    //                delete response;
    //            }

    //            artistsToJson(artistLinks, json);
                return json;
            }

            // byChar and byType has too many items - parse it all at once is not good idea ?

    //        QJsonArray byChar(QChar /*target_char*/, const SearchLimit & limitations) { // https://myzuka.org/Artist/5633/G-Playaz/Songs/Page
    //            //TODO: realize later
    //        }

    //        QJsonArray byType(ByTypeArg target_type, const SearchLimit & limitations) { // https://myzuka.org/Hits/2014 //https://myzuka.org/Hits/Top100Weekly //https://myzuka.org/Hits/Top100Monthly
    //            QList<QUrl> urls;

    //            switch (target_type) { // need to modify grab processing of folder support in model
    //                case sets:
    //                case hits: {
    //                    urls << QUrl();
    //                break;}
    //                case soundtracks: { // https://myzuka.org/Soundtracks/Page2 https://myzuka.org/Soundtracks/2014/Page6
    //                    // has too many pages - more than 270
    //                break;}
    //                case charts: {

    //                break;}
    //                case by_years: {

    //                break;}
    //                case hits: break; // http://zaycev.net/musicset/other/more.html?page=2
    //                default: return QJsonArray();
    //            }
    //            //TODO: stop if result not contains elements
    //        }

            inline QJsonArray popular(QString & /*genre*/) { return sQuery(QUrl(baseUrlStr()), songs1); }

        protected:
            QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://musicshara.ru") % predicate; }

            QString refresh_proc(Response * reply) {
                Html::Document parser(reply);
                Html::Set tracks = parser.find(".options a[itemprop='audio']");
                qDebug() << "IN RESTORING" << tracks.count();

                if (tracks.isEmpty())
                    return QString();
                else
                    return baseUrlStr(tracks.link());
            }
            QJsonArray search_postprocess(QString & predicate, QString & /*genre*/, const SearchLimit & limitations) {
                QString url_str = baseUrlStr(QStringLiteral("/search-page-%2-%1.html?ajax=yw1")).arg(encodeStr(predicate), page_offset_key);

                QJsonArray json;
                lQuery(url_str, json, songs1, limitations.count_page, limitations.start_page, limitations.total_limit);

//                while(json.size() > limitations.count)
//                    json.removeLast();

                return json;
            }

            bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
                Html::Document parser(reply);
                bool result = false;

                switch(jtype) {
                    case songs1: {
                        Html::Set songs = parser.find(".track");

                        Html::Selector artistSelector(".info .artist");
                        Html::Selector titleSelector(".info .title");
    //                    Html::Selector bitrateSelector(".info .bitrate");
                        Html::Selector durationSelector(".info .duration");

                        for(Html::Set::Iterator song = songs.begin(); song != songs.end(); song++) {
                            QJsonObject song_obj;

                            song_obj.insert(url_key, (*song) -> value(QStringLiteral("data-src")));

                            QString artist = (*song) -> find(&artistSelector).text();
                            QString title = (*song) -> find(&titleSelector).text();
                            title = artist % QStringLiteral(" - ") % title;
                            song_obj.insert(title_key, title);

                            song_obj.insert(duration_key, (*song) -> find(&durationSelector).text());
    //                        song_obj.insert(bitrate_key, (*song) -> find(&bitrateSelector) -> text());
                            song_obj.insert(skip_info_key, true);

                            json << song_obj;
                        }

                        result = !songs.isEmpty();
                    }

                    default: ;
                }

                if (removeReply) delete reply;
                return result;
            }
        private:
            friend class Singleton<MusicShara>;
            inline MusicShara() { }

            inline virtual ~MusicShara() {}
        };
    }
}

#endif // MUSICSHARA
