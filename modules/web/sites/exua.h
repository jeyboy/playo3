//#ifndef EXUA
//#define EXUA

//#include "igrabber_api.h"

//#define ITEMS_PER_PAGE 50
//#define MAX_PAGES_PER_ARTIST 2

//namespace Grabber {
//    class Exua : public IGrabberApi {
//        const QString search_predicate_song_token, search_predicate_artist_token;
////        const QString data_url_token, title_token, search_path_token, search_predicate_token;
//        const Html::Selector searchIndexSelector, songTrSelector /*, artistSelector, songSelector, linksSelector, table_columns_selector*/;
//    public:
//        static Exua * instance();
//        inline static void close() { delete self; }

//        inline QString name() const { return QStringLiteral("EXua"); }
//        inline Playo3::WebSubType siteType() { return Playo3::exua_site; }

//        QJsonArray search(QString & predicate, QString & genre, int genre_id, bool popular_items, bool by_artist, int count) {
//            QUrl url;

//            if (!predicate.isEmpty()) {
//                url = QUrl(baseUrlStr(
//                    (by_artist ? search_predicate_artist_token : search_predicate_song_token) % predicate
//                ));
//            } else if (!genre.isEmpty())
//                return byGenre(genre, genre_id);
//            else if (popular_items)
//                return popular();

//            if (url.isEmpty()) return QJsonArray();

//            WebManager * manager = 0;
//            bool isNew = WebManager::valid(manager);
//            QJsonArray json;

//            if (by_artist)
//                artistsToJson()
//            else {
//                QNetworkReply * response = manager -> getSync(QNetworkRequest(url));
//            }


////            Html::Document parser(response);

////            Html::Set tables = parser.find(&searchTablesSelector);

////            if (!tables.isEmpty()) { // at this time table with albums is ignored (second table in the list)
////                Html::Tag * artists_table = 0;
////                Html::Tag * songs_table = 0;

////                for(int i = 0; i < tables.count(); i++) {
////                    Html::Set columns = tables[i] -> find(&table_columns_selector);
////                    if (columns.count() == 3) {
////                        if (columns[0] -> text().isEmpty())
////                            artists_table = tables[i];
////                        else
////                            songs_table = tables[i];
////                    }
////                }

////                if (artists_table && count > 10) {
////                    QHash<QString, QString> artistLinks;
////                    artistsToJson(manager, artists_table -> findLinks(&artistSelector, artistLinks), json);
////                }

////                if (songs_table) {
////                    Html::Set songs = songs_table -> find(&songTrSelector);

////                    if (count < ITEMS_PER_PAGE)
////                        while(songs.size() > count)
////                            songs.removeLast();

////                    for(Html::Set::Iterator song = songs.begin(); song != songs.end(); song++) {
////                        QJsonObject track_obj;

////                        Html::Tag * artist_tag = (*song) -> find(&artistSelector).first();
////                        Html::Tag * track_tag = (*song) -> find(&songSelector).first();
////                        Html::Tag * size_tag = (*song) -> childTag("td", 2);

////                        if (!size_tag) {
////                            Logger::instance() -> writeToStream(
////                                QStringLiteral("Myzuka grabber"),
////                                QStringLiteral("Search: parsing of songs"),
////                                QStringLiteral("Some shit happened"), true
////                            );
////                            return json;
////                        }

////                        QString title = artist_tag -> text() % QStringLiteral(" - ") % track_tag -> text();
////                        track_obj.insert(title_key, title);
////                        track_obj.insert(size_key, prepareSize(size_tag -> text()));
////                        track_obj.insert(refresh_key, baseUrlStr(track_tag -> link()));

////                        json << track_obj;
////                    }
////                }
////            }

//            delete response;
//            if (isNew) delete manager;
//            return json;
//        }

//    protected:
////        inline QString prepareSize(QString size) {
////            return size.trimmed().replace("Мб", "Mb").replace("Кб", "Kb");
////        }

//        QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://www.ex.ua") % predicate; }

//        bool indexToJson(WebManager * manager, QJsonArray & json, QUrl baseUrl) {
//            QNetworkReply * reply = manager -> getSync(url);
//            Html::Document parser(reply);
//            Html::Set positions = parser.find(&searchIndexSelector);



////            Html::Set set;
////            Html::Tag * tag;
////            Html::Set tracks = parser.find("div[itemprop='tracks']");
////            Html::Selector urlSelector("span[data-url^'/Song']");
////            Html::Selector infoSelector(".data>text");
////            Html::Selector detailsSelector(".details>.time>text");
////            Html::Selector refreshSelector(".details a[href^'/Song']");

////            for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
////                QJsonObject track_obj;

////                tag = (*track) -> find(&urlSelector).first();
////                track_obj.insert(url_key, baseUrlStr(tag -> value(data_url_token)));
////                track_obj.insert(title_key, tag -> value(title_token).section(' ', 1));

////                set = (*track) -> find(&infoSelector);
////                if (!set.isEmpty()) {
////                    track_obj.insert(duration_key, set.first() -> text().section(' ', 0, 0));
////                    track_obj.insert(bitrate_key, set.last() -> text().section(' ', 0, 0));
////                }

////                set = (*track) -> find(&detailsSelector);
////                if (!set.isEmpty())
////                    track_obj.insert(size_key, prepareSize(set.first() -> text()));

////                set = (*track) -> find(&refreshSelector);
////                if (!set.isEmpty())
////                    track_obj.insert(refresh_key, baseUrlStr(set.first() -> link()));

////                json << track_obj;
////            }

////            if (removeReply) delete reply;
////            return !tracks.isEmpty();
//        }

//        bool toJson(QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
//            Html::Document parser(reply);
//            Html::Set positions = parser.find(&searchIndexSelector);

//            Html::Set attrs = parser.find("p>b[text='Исполнитель']<p");
//            for(Html::Set::Iterator attr = attrs.begin(); attr != attrs.end(); attr++) {
//                QString name = (*attr) -> childTag("b").text();

//                if (name == QStringLiteral("Исполнитель")) {

//                } else if (name == QStringLiteral("Альбом")) {

//                } else if (name == QStringLiteral("Стиль")) {

//                } else if (name == QStringLiteral("Битрейт")) {

//                } else if (name == QStringLiteral("Формат")) {

//                }
//            }


//            for(Html::Set::Iterator position = positions.begin(); position != positions.end(); position++) {
//                QString url = (*position).link();


//            }


////            Html::Set set;
////            Html::Tag * tag;
////            Html::Set tracks = parser.find("div[itemprop='tracks']");
////            Html::Selector urlSelector("span[data-url^'/Song']");
////            Html::Selector infoSelector(".data>text");
////            Html::Selector detailsSelector(".details>.time>text");
////            Html::Selector refreshSelector(".details a[href^'/Song']");

////            for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
////                QJsonObject track_obj;

////                tag = (*track) -> find(&urlSelector).first();
////                track_obj.insert(url_key, baseUrlStr(tag -> value(data_url_token)));
////                track_obj.insert(title_key, tag -> value(title_token).section(' ', 1));

////                set = (*track) -> find(&infoSelector);
////                if (!set.isEmpty()) {
////                    track_obj.insert(duration_key, set.first() -> text().section(' ', 0, 0));
////                    track_obj.insert(bitrate_key, set.last() -> text().section(' ', 0, 0));
////                }

////                set = (*track) -> find(&detailsSelector);
////                if (!set.isEmpty())
////                    track_obj.insert(size_key, prepareSize(set.first() -> text()));

////                set = (*track) -> find(&refreshSelector);
////                if (!set.isEmpty())
////                    track_obj.insert(refresh_key, baseUrlStr(set.first() -> link()));

////                json << track_obj;
////            }

////            if (removeReply) delete reply;
////            return !tracks.isEmpty();
//        }
//    private:
//        inline Exua() : IGrabberApi(), search_predicate_song_token("/search?original_id=3&s="), search_predicate_artist_token("/search?original_id=7513588&s="),
//            searchIndexSelector(Html::Selector("table.panel tr[id!'ad_block']")), songTrSelector(Html::Selector("table.list tr a[rel='nofollow']"))
//        {} /*, data_url_token(QStringLiteral("data-url")),
//            title_token(QStringLiteral("title")), search_path_token(QStringLiteral("/Search")),
//            search_predicate_token(QStringLiteral("searchText=")), searchTablesSelector(Html::Selector(".content table")),
//            songTrSelector(Html::Selector("a[href^'/Song']<tr")), artistSelector(Html::Selector("td a[href^'/Artist']")),
//            songSelector(Html::Selector("a[href^'/Song']")), linksSelector(Html::Selector("table a[href^'/Genre/']")),
//            table_columns_selector("tr th") { }*/

//        inline virtual ~Exua() {}

//        static Exua * self;
//    };
//}

//#endif // EXUA
