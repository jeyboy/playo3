#ifndef MYZUKA_ALBUM
#define MYZUKA_ALBUM

#include "igrabber_api.h"

namespace Grabber {
    class MyzukaAlbum : public IGrabberApi {
        const QString data_url_token = QStringLiteral("data-url");
        const QString title_token = QStringLiteral("title");
    public:
        static MyzukaAlbum * instance();
        inline static void close() { delete self; }

    protected:
        QString baseUrlStr(QString predicate = DEFAULT_PREDICATE_NAME) { return "https://myzuka.org" % predicate; }
        void toJson(QNetworkReply * reply, QJsonArray & json) {
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
                track_obj.insert(duration_key, set.first() -> text().section(' ', 0, 0));
                track_obj.insert(bitrate_key, set.last() -> text().section(' ', 0, 0));

                set = (*track) -> find(&detailsSelector);
                track_obj.insert(size_key, set.first() -> text().section(' ', 0, 0));

                set = (*track) -> find(&refreshSelector);
                track_obj.insert(refresh_key, set.first() -> link());

                track_ar << track_obj;
            }

            json.append(track_ar);
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
