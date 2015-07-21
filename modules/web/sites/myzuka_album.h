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

//    protected:
        QString baseUrlStr(QString predicate = DEFAULT_PREDICATE_NAME) { return "https://myzuka.org" % predicate; }
        void toJson(QIODevice * reply, QJsonArray & json) {
            Html::Document parser(reply);

            Html::Set set;
            Html::Tag * tag;
            Html::Selector trackSelector("div[itemprop='tracks']");
            Html::Set tracks = parser.find(&trackSelector);
            Html::Selector urlSelector("span[data-url^'/Song']");
            Html::Selector infoSelector(".data>text");
            Html::Selector detailsSelector(".details>.time>text");

            QJsonArray track_ar;
            for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                QJsonObject track_obj;

                tag = (*track) -> find(&urlSelector).first();
                track_obj.insert(url_key, baseUrlStr(tag -> value(data_url_token)));
                track_obj.insert(title_key, tag -> value(title_token).section(' ', 1));

                qDebug() << (*track);
                set = (*track) -> find(&infoSelector);
                track_obj.insert(duration_key, set.first() -> text());
    //            track_obj.insert(GRAB_FIELD_BITRATE, set.last() -> text().section(' ', 0, 0));

                set = (*track) -> find(&detailsSelector);
                track_obj.insert(size_key, set.first() -> text());

                track_ar << track_obj;
            }

            json.append(track_ar);
        }
    private:
    //    inline QString infoPredicate() { return ".data text"; }


    //    <div id="playerDiv5103163" class="player-inline" itemprop="tracks" itemscope="itemscope" itemtype="http://schema.org/MusicRecording">
    //                <div id="play_5103163" class="play">
    //                    <span class="ico " data-url="/Song/Play/5103163?t=635720680317170488&amp;s=5b6ced4159944069a244d6939edac3e2" title="Слушать Kadebostany feat. The Avener - Castle In The Snow"><!----></span>
    //                </div>
    //                    <div class="options">
    //                <div class="top">
    //                        <span id="add_5103163" class="ico-plus  " title="Добавить в плейлист"><!----></span>
    //                                        <a href="/Song/5103163/The-Avener-Castle-In-The-Snow" title="Скачать Kadebostany feat. The Avener - Castle In The Snow">Скачать</a>
    //                </div>
    //                    <div class="data">03:33 <span class="sep">|</span> 256 Кб/с</div>
    //            </div>
    //            <div class="details">
    //                    <div class="time">6,58 Мб</div>
    //                <a class="strong" href="/Artist/282015/Kadebostany">Kadebostany</a>                        feat.
    //                        <span itemprop="byArtist" itemscope="itemscope" itemtype="http://schema.org/MusicGroup">
    //                            <meta content="/Artist/282015/Kadebostany" itemprop="url">
    //                            <meta content="Kadebostany" itemprop="name">
    //                        </span>
    //    <a class="strong" href="/Artist/326640/The-Avener">The Avener</a>                    <span itemprop="byArtist" itemscope="itemscope" itemtype="http://schema.org/MusicGroup">
    //                            <meta content="/Artist/326640/The-Avener" itemprop="url">
    //                            <meta content="The Avener" itemprop="name">
    //                        </span>
    //                    <p>
    //                        <a href="/Song/5103163/The-Avener-Castle-In-The-Snow">Castle In The Snow</a>
    //                    </p>

    //                    <div class="icons">
    //                        <a href="/Song/5103163/The-Avener-Castle-In-The-Snow" class="ico-clip" title="Видео клип Kadebostany feat. The Avener - Castle In The Snow"><!----></a>
    //                        <a href="/Lyrics/5103163/The-Avener-Castle-In-The-Snow" class="ico-text" title="Текст песни Kadebostany feat. The Avener - Castle In The Snow"><!----></a>
    //                    </div>
    //            </div>
    //            <div class="player-controls">
    //                <div class="current-time"></div>
    //                <div class="timeline">
    //                    <div class="progress" style="width:0%;"><!----></div>
    //                </div>
    //                <div class="volume">
    //                    <div class="value" style="width:60%;"><!----></div>
    //                </div>
    //            </div>
    //        </div>

        inline MyzukaAlbum() : IGrabberApi() { }
        inline virtual ~MyzukaAlbum() {}

        static MyzukaAlbum * self;
    };
}

#endif // MYZUKA_ALBUM
