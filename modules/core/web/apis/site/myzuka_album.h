#ifndef MYZUKA_ALBUM
#define MYZUKA_ALBUM

#include "modules/core/web/interfaces/igrabber_api.h"
#include "modules/core/interfaces/singleton.h"

#define ITEMS_PER_PAGE 50
#define MAX_PAGES_PER_ARTIST 2

// store all selectors in global variables
namespace Core {
    namespace Web {

        using namespace Grabber;

        class MyzukaAlbum : public IGrabberApi, public Singleton<MyzukaAlbum> {
            const QString data_url_token, title_token, search_path_token, search_predicate_token;
            const Html::Selector searchTablesSelector, songTrSelector, artistSelector, songSelector, linksSelector, table_columns_selector;
        public:
            inline QString name() const { return QStringLiteral("Myzuka"); }
            inline SubType siteType() { return myzuka_site; }

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

            inline QJsonArray popular() { return sQuery(QUrl(baseUrlStr()), songs1); }

        protected:
            void prepareTables(Html::Set & tables, Html::Tag *& artists_table, Html::Tag *& songs_table) {
                for(int i = 0; i < tables.count(); i++) {
                    Html::Set columns = tables[i] -> find(&table_columns_selector);
                    if (columns.count() == 3) {
                        if (columns[0] -> text().isEmpty())
                            artists_table = tables[i];
                        else
                            songs_table = tables[i];
                    }
                }
            }

            inline QString prepareSize(QString size) {
                return size.trimmed().replace("Мб", "Mb").replace("Кб", "Kb");
            }

            QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("https://myzuka.org") % predicate; }


            inline void genres_prepocessing() { lQuery(baseUrlStr(QStringLiteral("/Genre/Page") % page_offset_key), genres1, STYLES_MAX_PAGE); }

            QString refresh_postprocess(Response * reply) {
                Html::Document parser(reply);
                Html::Set tracks = parser.find(".options a[itemprop='audio']");

                if (tracks.isEmpty())
                    return QString();
                else
                    return baseUrlStr(tracks.link());
            }
            QJsonArray search_postprocess(QString & predicate, QString & /*genre*/, const SearchLimit & limitations) {
                QUrl url = QUrl(baseUrlStr(search_path_token));
                url.setQuery(search_predicate_token % predicate);

                Response * response = Manager::prepare() -> followedGet(url);

                QJsonArray json;
                Html::Document parser(response);
                response -> deleteLater();

                Html::Set tables = parser.find(&searchTablesSelector);
                Html::Tag * artists_table = 0, * songs_table = 0;
                prepareTables(tables, artists_table, songs_table);

                if (limitations.by_artists() && artists_table) {
                    QHash<QString, QString> artistLinks;
                    artistsToJson(artists_table -> findLinks(&artistSelector, artistLinks), json);
                }

                if (!limitations.by_artists() && songs_table) {
                    Html::Set songs = songs_table -> find(&songTrSelector);

                    if (limitations.count < ITEMS_PER_PAGE)
                        while(songs.size() > limitations.count)
                            songs.removeLast();

                    for(Html::Set::Iterator song = songs.begin(); song != songs.end(); song++) {
                        QJsonObject track_obj;

                        Html::Tag * artist_tag = (*song) -> find(&artistSelector).first();
                        Html::Tag * track_tag = (*song) -> find(&songSelector).first();
                        Html::Tag * size_tag = (*song) -> childTag("td", 2);

                        if (!size_tag) {
                            Logger::obj().write(
                                QStringLiteral("Myzuka grabber"),
                                QStringLiteral("Search: parsing of songs"),
                                QStringLiteral("Some shit happened"), true
                            );
                            return json;
                        }

                        QString title = artist_tag -> text() % QStringLiteral(" - ") % track_tag -> text();
                        track_obj.insert(title_key, title);
                        track_obj.insert(size_key, prepareSize(size_tag -> text()));
                        track_obj.insert(refresh_key, baseUrlStr(track_tag -> link()));

                        json << track_obj;
                    }
                }

                return json;
            }



            void artistsToJson(QHash<QString, QString> & artists, QJsonArray & json) {
                for(QHash<QString, QString>::Iterator artist = artists.begin(); artist != artists.end(); artist++) {
                    QString artistPage = artist.key() % QStringLiteral("/Songs/Page") % page_offset_key;

                    lQuery(baseUrlStr(artistPage), json, songs1, MAX_PAGES_PER_ARTIST);
                }
            }

            bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
                Html::Document parser(reply);
                bool result = false;

                switch(jtype) {
                    case songs1: {
                        Html::Set set;
                        Html::Tag * tag;
                        Html::Set tracks = parser.find("div[itemprop='tracks']");
                        Html::Selector urlSelector("span[data-url^'/Song']");
                        Html::Selector infoSelector(".data>text");
                        Html::Selector detailsSelector(".details>.time>text");
                        Html::Selector refreshSelector(".details a[href^'/Song']");

                        for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                            QJsonObject track_obj;

                            tag = (*track) -> find(&urlSelector).first();
                            track_obj.insert(url_key, baseUrlStr(tag -> value(data_url_token)));
                            track_obj.insert(title_key, tag -> value(title_token).section(' ', 1));

                            set = (*track) -> find(&infoSelector);
                            if (!set.isEmpty()) {
                                track_obj.insert(duration_key, set.text().section(' ', 0, 0));
                                track_obj.insert(bitrate_key, set.last() -> text().section(' ', 0, 0));
                            }

                            set = (*track) -> find(&detailsSelector);
                            if (!set.isEmpty())
                                track_obj.insert(size_key, prepareSize(set.text()));

                            set = (*track) -> find(&refreshSelector);
                            if (!set.isEmpty())
                                track_obj.insert(refresh_key, baseUrlStr(set.link()));

                            json << track_obj;
                        }

                        result = !tracks.isEmpty();
                    }

                    case genres1: {
                        Html::Set links = parser.find(&linksSelector);

                        for(Html::Set::Iterator link = links.begin(); link != links.end(); link++) {
                            QStringList list = (*link) -> link().split('/', QString::SkipEmptyParts);
                            genres.addGenre(list[2], list[1].toInt());
                        }
                        result = !links.isEmpty();
                    }

                    default: ;
                }

                if (removeReply) delete reply;
                return result;
            }
        private:
            friend class Singleton<MyzukaAlbum>;
            inline MyzukaAlbum() : data_url_token(QStringLiteral("data-url")),
                title_token(QStringLiteral("title")), search_path_token(QStringLiteral("/Search")),
                search_predicate_token(QStringLiteral("searchText=")), searchTablesSelector(Html::Selector(".content table")),
                songTrSelector(Html::Selector("a[href^'/Song']<tr")), artistSelector(Html::Selector("td a[href^'/Artist']")),
                songSelector(Html::Selector("a[href^'/Song']")), linksSelector(Html::Selector("table a[href^'/Genre/']")),
                table_columns_selector("tr th") { }

            inline virtual ~MyzukaAlbum() {}
        };
    }
}

#endif // MYZUKA_ALBUM
