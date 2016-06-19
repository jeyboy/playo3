#ifndef ZAYCEV
#define ZAYCEV

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/iapi.h"
#include "modules/core/interfaces/isource.h"
#include "modules/core/web/grabber_keys.h"

// store all selectors in global variables
namespace Core {
    namespace Web {
        class Zaycev : public ISource, public IApi, public Singleton<Zaycev> {
            QHash<QString, QString> letters {
                {QStringLiteral("1"), QStringLiteral("0")}, {QStringLiteral("2"), QStringLiteral("0")}, {QStringLiteral("3"), QStringLiteral("0")},
                {QStringLiteral("4"), QStringLiteral("0")}, {QStringLiteral("5"), QStringLiteral("0")}, {QStringLiteral("6"), QStringLiteral("0")},
                {QStringLiteral("7"), QStringLiteral("0")}, {QStringLiteral("8"), QStringLiteral("0")}, {QStringLiteral("9"), QStringLiteral("0")},

                {QStringLiteral("а"), QStringLiteral("rus-a")}, {QStringLiteral("б"), QStringLiteral("rus-b")}, {QStringLiteral("в"), QStringLiteral("rus-v")},
                {QStringLiteral("г"), QStringLiteral("rus-g")}, {QStringLiteral("д"), QStringLiteral("rus-d")}, {QStringLiteral("е"), QStringLiteral("rus-e")},
                {QStringLiteral("ё"), QStringLiteral("rus-e")}, {QStringLiteral("ж"), QStringLiteral("rus-zh")}, {QStringLiteral("з"), QStringLiteral("rus-z")},
                {QStringLiteral("и"), QStringLiteral("rus-i")}, {QStringLiteral("к"), QStringLiteral("rus-k")}, {QStringLiteral("л"), QStringLiteral("rus-l")},
                {QStringLiteral("м"), QStringLiteral("rus-m")}, {QStringLiteral("н"), QStringLiteral("rus-n")}, {QStringLiteral("о"), QStringLiteral("rus-o")},
                {QStringLiteral("п"), QStringLiteral("rus-p")}, {QStringLiteral("р"), QStringLiteral("rus-r")}, {QStringLiteral("с"), QStringLiteral("rus-s")},
                {QStringLiteral("т"), QStringLiteral("rus-t")}, {QStringLiteral("у"), QStringLiteral("rus-u")}, {QStringLiteral("ф"), QStringLiteral("rus-f")},
                {QStringLiteral("х"), QStringLiteral("rus-h")}, {QStringLiteral("ц"), QStringLiteral("rus-c")}, {QStringLiteral("ч"), QStringLiteral("rus-ch")},
                {QStringLiteral("ш"), QStringLiteral("rus-sh")}, {QStringLiteral("щ"), QStringLiteral("rus-sch")}, {QStringLiteral("э"), QStringLiteral("rus-ee")},
                {QStringLiteral("ю"), QStringLiteral("rus-yu")}, {QStringLiteral("я"), QStringLiteral("rus-ya")}
            };
            PolyQueryRules rules(const SearchLimit & limits) {
                return PolyQueryRules(call_iter_type_page,
                    limits.start_offset == 0 ? 1 : limits.start_offset,
                    limits.items_limit,
                    limits.requests_limit
                );
            }
        public:
            inline QString name() const { return QStringLiteral("Zaycev"); }
            inline DataSubType siteType() const { return dt_site_zaycev; }

            QJsonArray searchByGenre(const SearchLimit & limits) { // default is popular // INFO: not finished
                return pRequest(
                    QStringLiteral("http://zaycev.net/genres/%1/%2_%3.html")
                        .arg(limits.genre, // need to convert genre !!!
                             limits.by_newest() ? QStringLiteral("new") : QStringLiteral("index"),
                             OFFSET_TEMPLATE),
                    call_type_html, rules(limits), 0, proc_tracks1
                );
            }

            // http://zaycev.net/artist/letter-rus-zh-more.html?page=1
            QJsonArray searchByChar(const SearchLimit & limits) { // by default - sort by popularity
                return pRequest(
                    QStringLiteral("http://zaycev.net/artist/letter-%1-more.html?page=%2")
                        .arg(prepareLetter(limits), OFFSET_TEMPLATE),
                    call_type_html, rules(limits), 0, proc_char1
                );
            }

            QJsonArray searchInSets(const SearchLimit & limits) { // required on loadable containers // not finished
                return pRequest(
                    QStringLiteral("http://zaycev.net/musicset%1/more.html?page=%2")
                        .arg("", OFFSET_TEMPLATE), // "/news", "/epochs", "/zhanry", "/soundtrack", "/national", "/holiday", "/mood", "/top100", "/other"
                    call_type_html, rules(limits), 0, proc_set1
                );
            }

            QJsonArray loadSet(const QVariantHash & attrs) {
                return saRequest(
                    attrs.value(tkn_grab_refresh).toString(),
                    call_type_html,
                    0,
                    (AdditionalProc)attrs.value(tkn_grab_set_parser, proc_tracks1).toInt()
                );
            }

            QJsonArray newest(const SearchLimit & limits) {
                return pRequest(
                    QStringLiteral("http://zaycev.net/new/more.html?page=") % OFFSET_TEMPLATE,
                    call_type_html, rules(limits), 0, proc_tracks1
                );
            }

            QJsonArray popular(const SearchLimit & limits) {
                return pRequest(
                    QStringLiteral("http://zaycev.net/top/more.html?page=") % OFFSET_TEMPLATE,
                    call_type_html, rules(limits), 0, proc_tracks1
                );

//                return saRequest(baseUrlStr(), call_type_html, 0, proc_tracks1);
            }

        protected:
            QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) { return QStringLiteral("http://zaycev.net") % predicate; }

            bool parseTracks(QueriableArg * arg, const Html::Document & parser, const Html::Selector & artist_selector) {
                QString ban_class = QStringLiteral("track-is-banned");
                Html::Set tracks = parser.find(".musicset-track-list__items .musicset-track"); //.track-is-banned // banned track is not playable for some countries
                QString data_url = QStringLiteral("data-url");
                Html::Selector title_selector(".musicset-track__track-name a");

                qDebug() << "TRACKS AMOUNT" << tracks.size();
                for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                    qDebug() << "TRACKS" << (*track) -> hasClass(ban_class);
                    if ((*track) -> hasClass(ban_class)) continue;

                    QJsonObject track_obj;

                    track_obj.insert(tkn_grab_refresh, baseUrlStr((*track) -> value(data_url)));
                    track_obj.insert(tkn_grab_duration, Duration::fromSeconds((*track) -> value(QStringLiteral("data-duration")).toInt()));
                    track_obj.insert(tkn_skip_info, true);


                    QString artist = (*track) -> findFirst(&artist_selector) -> text();
                    QString title = (*track) -> findFirst(&title_selector) -> text();
                    title = artist % QStringLiteral(" - ") % title;
                    track_obj.insert(tkn_grab_title, title);

                    arg -> append(track_obj, track + 1 == tracks.end());
                }

                return !tracks.isEmpty();
            }

            bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply = false) {
                if (reply -> status() != 200) return false; // if pagination overlimited - 302 status received

                Html::Document parser = reply -> toHtml(removeReply);
                bool result = false;

                switch(arg -> post_proc) {
                    case proc_char1: {
                        result = parseTracks(arg, parser, Html::Selector("< .artist-item__info .artist-item-info__top a"));
                    break;}
                    case proc_tracks1: {
                        result = parseTracks(arg, parser, Html::Selector(".musicset-track__artist a"));
                    break;}

                    case proc_set1: {
                        Html::Set sets = parser.find(".musicset-list__item");
                        Html::Selector title_selector(".musicset-item__title a");
                        Html::Selector link_selector(".musicset-item__pic .musicset-item__pic-link");
                        Html::Selector tracks_amount(".musicset-item__details p");

                        QUrl base_url(QStringLiteral("http://zaycev.net"));
                        QString digit;

                        qDebug() << "SETS AMOUNT" << sets.size();
                        for(Html::Set::Iterator set = sets.begin(); set != sets.end(); set++) {
                            QJsonObject set_obj;

                            set_obj.insert(tkn_grab_is_set, true);
                            set_obj.insert(tkn_grab_set_parser, proc_tracks1);
                            set_obj.insert(tkn_grab_title, (*set) -> findFirst(&title_selector) -> text());

                            Html::Tag * link_tag = (*set) -> findFirst(&link_selector);

                            set_obj.insert(
                                tkn_grab_refresh,
                                base_url.resolved(QUrl(link_tag -> link())).toString()
                            );

                            set_obj.insert(
                                tkn_grab_art_url,
                                base_url.resolved(QUrl(link_tag -> findFirst("img") -> value(QStringLiteral("src")))).toString()
                            );

                            if (Info::extractNumber((*set) -> findFirst(&tracks_amount) -> text(), digit))
                                set_obj.insert(
                                    tkn_grab_set_items_amount,
                                    digit.toInt()
                                );

                            arg -> append(set_obj, set + 1 == sets.end());
                        }

                        return !sets.isEmpty();
                    break;}

                    default: ;
                }

                return result;
            }

            QString prepareLetter(const SearchLimit & limits) {
                QString letter = limits.predicate.mid(0, 1);
                if (letters.contains(letter))
                    return letters[letter];

                return letter;
            }

            void genresProc() { // manual init at this time
                genres.addGenre(QStringLiteral("pop"), 13);
                genres.addGenre(QStringLiteral("rock"), 17);
                genres.addGenre(QStringLiteral("rap"), 15);
                genres.addGenre(QStringLiteral("alternative"), 20);
                genres.addGenre(QStringLiteral("electronic"), 52);
                genres.addGenre(QStringLiteral("shanson"), 102);
                genres.addGenre(QStringLiteral("soundtrack"), 24);
                genres.addGenre(QStringLiteral("metal"), 9);
                genres.addGenre(QStringLiteral("classical"), 32);
                genres.addGenre(QStringLiteral("dance"), 3);
                genres.addGenre(QStringLiteral("easy"), 98);
                genres.addGenre(QStringLiteral("rnb"), 14);
                genres.addGenre(QStringLiteral("jazz"), 8);
                genres.addGenre(QStringLiteral("reggae"), 16);
                genres.addGenre(QStringLiteral("other"), 12);
            }

            // {"url":"http://dl.zaycev.net/85673/2745662/rick_ross_-_love_sosa.mp3?dlKind=play&format=json"}
            inline QString refreshProc(Response * reply) {
                return reply -> toJson().value(QStringLiteral("url")).toString();
            }

            QJsonArray searchProc(const SearchLimit & limits) {
                QString url_str = baseUrlStr(
                    QStringLiteral("/search.html?query_search=%1&page=%2")
                        .arg(encodeStr(limits.predicate), OFFSET_TEMPLATE)
                );

                PolyQueryRules rules(
                    call_iter_type_page, limits.start_offset,
                    qMin(limits.items_limit, DEFAULT_ITEMS_LIMIT), qMin(limits.requests_limit, 2)
                );
                return pRequest(url_str, call_type_html, rules, 0, proc_tracks1);


//                QJsonArray json;
//                lQuery(url_str, json, proc_tracks1, qMin(limitations.count_page, 2), limitations.start_page, limitations.total_limit); // 2 pages at this time
//                return json;
            }
        private:
            friend class Singleton<Zaycev>;
            inline Zaycev() {}
            inline virtual ~Zaycev() {}
        };
    }
}

#endif // ZAYCEV
