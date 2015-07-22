#ifndef MYZUKA_ALBUM
#define MYZUKA_ALBUM

#include "igrabber_api.h"

#define ITEMS_PER_PAGE 50

namespace Grabber {
    class MyzukaAlbum : public IGrabberApi {
        const QString data_url_token = QStringLiteral("data-url");
        const QString title_token = QStringLiteral("title");
    public:
        static MyzukaAlbum * instance();
        inline static void close() { delete self; }

        QJsonArray search(QString & predicate, QString & genre, bool popular, int count) {
            QUrl url;

            if (!predicate.isEmpty())
                url = QUrl(baseUrlStr())
            else if (!genre.isEmpty())

            else return popular();

            bool isNew = !manager ? WebManager::valid(manager) : false;
            QNetworkReply * response = manager -> getSync(QNetworkRequest(url));
            search_postprocess(response, arr);
            delete response;
            if (isNew) delete manager;
        }

//        TargetGenres genresList() { return TargetGenres(); } // https://myzuka.org/Genre/Page1

        QJsonArray byGenre(QString /*genre*/, int /*genre_code*/ = 0) { // https://myzuka.org/Genre/92/8-Bit

        }

        QJsonArray byChar(QChar /*target_char*/) { // https://myzuka.org/Artist/5633/G-Playaz/Songs

        }

//        QJsonArray byType(QString /*target_type*/) {

//        }

        QJsonArray popular() {
            bool isNew = !manager ? WebManager::valid(manager) : false;
            QJsonArray res;
            toJson(manager -> getSync(QNetworkRequest(QUrl(baseUrlStr()))), res, true);
            if (isNew) delete manager;
            return res;
        }

    protected:
        QString baseUrlStr(QString predicate = DEFAULT_PREDICATE_NAME) { return "https://myzuka.org" % predicate; }
        bool toJson(QNetworkReply * reply, QJsonArray & json, bool removeReply = false) {
            Html::Document parser(reply);

            Html::Set set;
            Html::Tag * tag;
            Html::Selector trackSelector("div[itemprop='tracks']");
            Html::Set tracks = parser.find(&trackSelector);
            Html::Selector urlSelector("span[data-url^'/Song']");
            Html::Selector infoSelector(".data>text");
            Html::Selector detailsSelector(".details>.time>text");
            Html::Selector refreshSelector(".details a[href^'/Song']");

            QJsonArray track_ar;
            for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                QJsonObject track_obj;

                tag = (*track) -> find(&urlSelector).first();
                track_obj.insert(url_key, baseUrlStr(tag -> value(data_url_token)));
                track_obj.insert(title_key, tag -> value(title_token).section(' ', 1));

                set = (*track) -> find(&infoSelector);
                if (!set.isEmpty()) {
                    track_obj.insert(duration_key, set.first() -> text().section(' ', 0, 0));
                    track_obj.insert(bitrate_key, set.last() -> text().section(' ', 0, 0));
                }

                set = (*track) -> find(&detailsSelector);
                if (!set.isEmpty())
                    track_obj.insert(size_key, set.first() -> text().section(' ', 0, 0));

                set = (*track) -> find(&refreshSelector);
                if (!set.isEmpty())
                    track_obj.insert(refresh_key, set.first() -> link());

                track_ar << track_obj;
            }

            json.append(track_ar);
            if (removeReply) delete reply;
            return !tracks.isEmpty();
        }

        QString refresh_postprocess(QNetworkReply * reply) {
            Html::Document parser(reply);

            Html::Selector trackSelector(".options a[itemprop='audio']");
            Html::Set tracks = parser.find(&trackSelector);

            if (tracks.isEmpty())
                return QString();
            else
                return baseUrlStr(tracks.first() -> link());
        }
    private:
        inline MyzukaAlbum() : IGrabberApi() { }
        inline virtual ~MyzukaAlbum() {}

        static MyzukaAlbum * self;
    };
}

#endif // MYZUKA_ALBUM
