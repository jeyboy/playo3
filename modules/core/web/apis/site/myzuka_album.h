#ifndef MYZUKA_ALBUM
#define MYZUKA_ALBUM

#include "modules/core/interfaces/isource.h"
#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/iapi.h"
#include "modules/core/web/grabber_keys.h"

#define ITEMS_PER_PAGE 50

// store all selectors in global variables
namespace Core {
    namespace Web {
        class MyzukaAlbum : public ISource, public IApi, public Singleton<MyzukaAlbum> {
            const QString data_url_token, title_token, search_path_token, search_predicate_token;
            const Html::Selector searchTablesSelector, songTrSelector, artistSelector, songSelector, linksSelector, table_columns_selector;
        public:
            inline QString name() const { return QStringLiteral("Myzuka"); }
            inline DataSubType siteType() const { return dt_site_myzuka; }

            // artists by genre
            QJsonArray searchByGenre(const SearchLimit & /*limits*/) { // https://myzuka.org/Genre/92/8-Bit https://myzuka.org/Genre/11/Pop/Page2
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

    //        QJsonArray searchByChar(const SearchLimit & limits) { // https://myzuka.org/Artist/5633/G-Playaz/Songs/Page
    //            //TODO: realize later
    //        }

    //        QJsonArray searchByType(const SearchLimit & limits) { // https://myzuka.org/Hits/2014 //https://myzuka.org/Hits/Top100Weekly //https://myzuka.org/Hits/Top100Monthly
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

            inline QJsonArray popular(const SearchLimit & /*limits*/) {
                return saRequest(baseUrlStr(), call_type_html, 0, proc_tracks1);
//                return sQuery(QUrl(baseUrlStr()), proc_songs1);
            }

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

            inline QString prepareSize(const QString & size) {
                return size.trimmed().replace("Мб", "Mb").replace("Кб", "Kb");
            }

            QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) {
                return QStringLiteral("https://myzuka.org") % predicate;
            }


            inline void genresProc() { // not tested
                PolyQueryRules rules(call_iter_type_page, call_iter_method_offset);
                pRequest(baseUrlStr(QStringLiteral("/Genre/Page") % OFFSET_TEMPLATE), call_type_html, rules, 0, proc_genres1);

//                lQuery(baseUrlStr(QStringLiteral("/Genre/Page") % OFFSET_TEMPLATE), proc_genres1, DEFAULT_REQUESTS_LIMIT);
            }

            QString refreshProc(Response * reply) {
                Html::Document doc = reply -> toHtml();
                Html::Set tracks = doc.find(".options a[itemprop='audio']");

                if (tracks.isEmpty())
                    return QString();
                else
                    return baseUrlStr(tracks.link());
            }
            QJsonArray searchProc(const SearchLimit & limits) {
                QUrl url = QUrl(baseUrlStr(search_path_token));
                url.setQuery(search_predicate_token % limits.predicate);

                QJsonArray arr;
                Response * response = Manager::prepare() -> getFollowed(url);
                Html::Document parser = response -> toHtml();

                Html::Set tables = parser.find(&searchTablesSelector);
                Html::Tag * artists_table = 0, * songs_table = 0;
                prepareTables(tables, artists_table, songs_table);

                if (limits.by_artists() && artists_table) {
                    QHash<QString, QString> artistLinks;
                    artistsToJson(artists_table -> findLinks(&artistSelector, artistLinks), arr);
                }

                if (!limits.by_artists() && songs_table) {
                    Html::Set songs = songs_table -> find(&songTrSelector);

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
                            return arr;
                        }

                        QString title = artist_tag -> text() % QStringLiteral(" - ") % track_tag -> text();
                        track_obj.insert(tkn_grab_title, title);
                        track_obj.insert(tkn_skip_info, true);
                        track_obj.insert(tkn_grab_size, prepareSize(size_tag -> text()));
                        track_obj.insert(tkn_grab_refresh, baseUrlStr(track_tag -> link()));

                        arr << track_obj;
                    }
                }

                return arr;
            }



            void artistsToJson(QHash<QString, QString> & artists, QJsonArray & arr) {
                for(QHash<QString, QString>::Iterator artist = artists.begin(); artist != artists.end(); artist++) {
                    QString artistPage = artist.key() % QStringLiteral("/Songs/Page") % OFFSET_TEMPLATE;

                    PolyQueryRules rules(call_iter_type_page, call_iter_method_offset, DEFAULT_ITEMS_LIMIT, MAX_PAGES_PER_ARTIST);
                    pRequest(baseUrlStr(artistPage), call_type_html, rules, &arr, proc_tracks1);

//                    lQuery(baseUrlStr(artistPage), arr, proc_songs1, MAX_PAGES_PER_ARTIST);
                }
            }

            bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply = false) {
//            bool toJson(toJsonType jtype, QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
                Html::Document parser = reply -> toHtml(removeReply);
                bool result = false;

                switch(arg -> post_proc) {
                    case proc_tracks1: {
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
                            track_obj.insert(tkn_grab_url, baseUrlStr(tag -> value(data_url_token)));
                            track_obj.insert(tkn_grab_title, tag -> value(title_token).section(' ', 1));

                            set = (*track) -> find(&infoSelector);
                            if (!set.isEmpty()) {
                                track_obj.insert(tkn_grab_duration, set.text().section(' ', 0, 0));
                                track_obj.insert(tkn_grab_bitrate, set.last() -> text().section(' ', 0, 0));
                            }

                            set = (*track) -> find(&detailsSelector);
                            if (!set.isEmpty())
                                track_obj.insert(tkn_grab_size, prepareSize(set.text()));

                            set = (*track) -> find(&refreshSelector);
                            if (!set.isEmpty())
                                track_obj.insert(tkn_grab_refresh, baseUrlStr(set.link()));

                            arg -> append(track_obj, track + 1 == tracks.end());
                        }

                        result = !tracks.isEmpty();
                    }

                    case proc_genres1: {
                        Html::Set links = parser.find(&linksSelector);

                        for(Html::Set::Iterator link = links.begin(); link != links.end(); link++) {
                            QStringList list = (*link) -> link().split('/', QString::SkipEmptyParts);
                            genres.addGenre(list[2], list[1].toInt());
                        }
                        result = !links.isEmpty();
                    }

                    default: ;
                }

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
