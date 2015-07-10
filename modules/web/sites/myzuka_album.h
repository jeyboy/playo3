#ifndef MYZUKA_ALBUM
#define MYZUKA_ALBUM

#include "igrabber_api.h"

class MyzukaAlbum : public  IGrabberApi {
protected:
    QString baseUrlStr(QString & predicate = DEFAULT_PREDICATE_NAME) { return "https://myzuka.org" + predicate; }
    void toJson(QNetworkReply * reply, QJsonObject & json) {
        HtmlParser parser(reply);

//        HtmlSet set;
        HtmlTag * tag;
        HtmlSelector * trackSelector = HtmlSelector::build(trackPredicate());
        HtmlSet tracks = parser.find(trackSelector);
        HtmlSelector * urlSelector = HtmlSelector::build(urlPredicate());
//        HtmlSelector * infoSelector = HtmlSelector::build(infoPredicate());

        QJsonArray track_ar;
        for(HtmlSet::Iterator track = tracks.begin(); track != tracks.end(); track++){
            QJsonObject track_obj;

            tag = (*track) -> find(urlSelector).first();
            qDebug() << "TAG" << tag;
            track_obj.insert("url", baseUrlStr(tag -> value("data-url")));
            track_obj.insert("title", tag -> value("title").section(' ', 1));

//            set = (*track) -> find(infoSelector);
//            track_obj.insert("time", set.first() -> text());
//            track_obj.insert("bitrate", set.last() -> text().section(' ', 0, 0));
//            track_obj.insert("bitrate", set.last() -> text().section(' ', 0, 0));

            track_ar << track_obj;
        }

        delete trackSelector;

        json.insert("audio", track_ar);
        return res;
    }
private:
    inline QString trackPredicate() { return "div[itemprop='tracks']"; }
    inline QString urlPredicate() { return "span[data-url^'/Song']"; }
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
};

#endif // MYZUKA_ALBUM
