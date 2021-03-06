#ifndef ZAYCEV
#define ZAYCEV

#include "modules/core/interfaces/singleton.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/interfaces/isource.h"

// store all selectors in global variables
namespace Core {
    namespace Web {
        class Zaycev : public ISource, public IQueriable, public Singleton<Zaycev> {
            QHash<QString, QString> letters {
                {LSTR("1"), LSTR("0")}, {LSTR("2"), LSTR("0")}, {LSTR("3"), LSTR("0")},
                {LSTR("4"), LSTR("0")}, {LSTR("5"), LSTR("0")}, {LSTR("6"), LSTR("0")},
                {LSTR("7"), LSTR("0")}, {LSTR("8"), LSTR("0")}, {LSTR("9"), LSTR("0")},

                {LSTR("а"), LSTR("rus-a")},  {LSTR("б"), LSTR("rus-b")},   {LSTR("в"), LSTR("rus-v")},
                {LSTR("г"), LSTR("rus-g")},  {LSTR("д"), LSTR("rus-d")},   {LSTR("е"), LSTR("rus-e")},
                {LSTR("ё"), LSTR("rus-e")},  {LSTR("ж"), LSTR("rus-zh")},  {LSTR("з"), LSTR("rus-z")},
                {LSTR("и"), LSTR("rus-i")},  {LSTR("к"), LSTR("rus-k")},   {LSTR("л"), LSTR("rus-l")},
                {LSTR("м"), LSTR("rus-m")},  {LSTR("н"), LSTR("rus-n")},   {LSTR("о"), LSTR("rus-o")},
                {LSTR("п"), LSTR("rus-p")},  {LSTR("р"), LSTR("rus-r")},   {LSTR("с"), LSTR("rus-s")},
                {LSTR("т"), LSTR("rus-t")},  {LSTR("у"), LSTR("rus-u")},   {LSTR("ф"), LSTR("rus-f")},
                {LSTR("х"), LSTR("rus-h")},  {LSTR("ц"), LSTR("rus-c")},   {LSTR("ч"), LSTR("rus-ch")},
                {LSTR("ш"), LSTR("rus-sh")}, {LSTR("щ"), LSTR("rus-sch")}, {LSTR("э"), LSTR("rus-ee")},
                {LSTR("ю"), LSTR("rus-yu")}, {LSTR("я"), LSTR("rus-ya")}
            };
            PolyQueryRules rules(const SearchLimit & limits) {
                int start_offset = qMax(limits.start_offset.toInt(), 1);

                return PolyQueryRules(call_iter_type_page,
                    start_offset,
                    limits.items_limit,
                    limits.requests_limit
                );
            }
        public:
            inline QString name() const { return LSTR("Zaycev"); }
            inline DataSubType sourceType() const { return dt_web_zaycev; }

            QJsonValue searchByGenre(const SearchLimit & limits) { // default is popular // INFO: not finished
                QueriableResponse response = pRequest(
                    QStringLiteral("http://zaycev.net/genres/%1/%2_%3.html")
                        .arg(limits.genre, // need to convert genre !!!
                             limits.by_newest() ? LSTR("new") : LSTR("index"),
                             OFFSET_TEMPLATE),
                    call_type_html, rules(limits), 0, proc_tracks1
                );

                return prepareBlock(dmt_audio, cmd_mtd_unknown, response, limits);
            }

            // http://zaycev.net/artist/letter-rus-zh-more.html?page=1
            QJsonValue searchByChar(const SearchLimit & limits) { // by default - sort by popularity
                QueriableResponse response = pRequest(
                    QStringLiteral("http://zaycev.net/artist/letter-%1-more.html?page=%2")
                        .arg(prepareLetter(limits), OFFSET_TEMPLATE),
                    call_type_html, rules(limits), 0, proc_char1
                );

                return prepareBlock(dmt_audio, cmd_mtd_unknown, response, limits);
            }

            QJsonValue searchInSets(const SearchLimit & limits) { // required on loadable containers // not finished
                QueriableResponse response = pRequest(
                    QStringLiteral("http://zaycev.net/musicset%1/more.html?page=%2")
                        .arg("", OFFSET_TEMPLATE), // "/news", "/epochs", "/zhanry", "/soundtrack", "/national", "/holiday", "/mood", "/top100", "/other"
                    call_type_html, rules(limits), 0, proc_set1
                );

                return prepareBlock(dmt_audio, cmd_mtd_unknown, response, limits);
            }

            QJsonValue loadSetData(const QString & attrs) {
                QUrlQuery query(attrs);

                return saRequest(
                    query.queryItemValue(CMD_ID),
                    call_type_html,
                    0,
                    (AdditionalProc)query.queryItemValue(CMD_PARSER).toInt()
                );
            }

            QJsonValue newest(const SearchLimit & limits) {
                QueriableResponse response = pRequest(
                    LSTR("http://zaycev.net/new/more.html?page=") % OFFSET_TEMPLATE,
                    call_type_html, rules(limits), 0, proc_tracks1
                );

                return prepareBlock(dmt_audio, cmd_mtd_unknown, response, limits);
            }

            QJsonValue popular(const SearchLimit & limits) {
                QueriableResponse response = pRequest(
                    LSTR("http://zaycev.net/top/more.html?page=") % OFFSET_TEMPLATE,
                    call_type_html, rules(limits), 0, proc_tracks1
                );

                return prepareBlock(dmt_audio, cmd_mtd_unknown, response, limits);
            }

        protected:
            QString baseUrlStr(const QuerySourceType & /*stype*/, const QString & predicate = DEFAULT_PREDICATE_NAME) { return LSTR("http://zaycev.net") % predicate; }

            bool parseTracks(QueriableArg * arg, const Html::Document & parser, const Html::Selector & artist_selector) {
                QString ban_class = LSTR("track-is-banned");
                Html::Set tracks = parser.find(".musicset-track-list__items .musicset-track"); //.track-is-banned // banned track is not playable for some countries
                QString data_url = LSTR("data-url");
                Html::Selector title_selector(".musicset-track__track-name a");

                qDebug() << "TRACKS AMOUNT" << tracks.size();
                for(Html::Set::Iterator track = tracks.begin(); track != tracks.end(); track++) {
                    qDebug() << "TRACKS" << (*track) -> hasClass(ban_class);
                    if ((*track) -> hasClass(ban_class)) continue;

                    QJsonObject track_obj;

                    track_obj.insert(tkn_grab_refresh, baseUrlStr(qst_site, (*track) -> value(data_url)));
                    track_obj.insert(tkn_grab_duration, Duration::fromSeconds((*track) -> value(LSTR("data-duration")).toInt()));
                    track_obj.insert(tkn_skip_info, true);

                    Html::Tag * artist_tag = (*track) -> findFirst(&artist_selector);
                    Html::Tag * title_tag = (*track) -> findFirst(&title_selector);

                    if (artist_tag && title_tag) {
                        QString artist = artist_tag -> text();
                        QString title = title_tag -> text();
                        title = artist % tkn_dash % title;
                        track_obj.insert(tkn_grab_title, title);
                    } else {
                        qDebug() << (*(*track));
                    }

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
                        result = parseTracks(arg, parser, Html::Selector(".musicset-track__artist *")); // a or span
                    break;}

                    case proc_set1: {
                        Html::Set sets = parser.find(".musicset-list__item");
                        Html::Selector title_selector(".musicset-item__title a");
                        Html::Selector link_selector(".musicset-item__pic .musicset-item__pic-link");
//                        Html::Selector tracks_amount(".musicset-item__details p");

                        QUrl base_url(LSTR("http://zaycev.net"));
//                        QString digit;

                        qDebug() << "SETS AMOUNT" << sets.size();
                        for(Html::Set::Iterator set = sets.begin(); set != sets.end(); set++) {
                            QJsonObject set_obj;
                            Html::Tag * link_tag = (*set) -> findFirst(&link_selector);

//                            set_obj.insert(tkn_grab_is_set, true);
                            set_obj.insert(tkn_media_type, dmt_audio_set);

                            set_obj.insert(
                                tkn_loadable_cmd,
                                Cmd::build(
                                    sourceType(), cmd_mtd_load_set_data,
                                    {
                                        {CMD_ID, base_url.resolved(QUrl(link_tag -> link())).toString()},
                                        {CMD_PARSER, QString::number(proc_tracks1)}
                                    }
                                ).toString()
                            );

                            set_obj.insert(tkn_grab_title, (*set) -> findFirst(&title_selector) -> text());



//                            set_obj.insert(
//                                tkn_grab_refresh,
//                                base_url.resolved(QUrl(link_tag -> link())).toString()
//                            );

                            set_obj.insert(
                                tkn_grab_art_url,
                                base_url.resolved(QUrl(link_tag -> findFirst("img") -> src())).toString()
                            );

//                            if (Info::extractNumber((*set) -> findFirst(&tracks_amount) -> text(), digit))
//                                set_obj.insert(
//                                    tkn_grab_set_items_amount,
//                                    digit.toInt()
//                                );

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
                genres.addGenre(LSTR("pop"),            13);
                genres.addGenre(LSTR("rock"),           17);
                genres.addGenre(LSTR("rap"),            15);
                genres.addGenre(LSTR("alternative"),    20);
                genres.addGenre(LSTR("electronic"),     52);
                genres.addGenre(LSTR("shanson"),        102);
                genres.addGenre(LSTR("soundtrack"),     24);
                genres.addGenre(LSTR("metal"),          9);
                genres.addGenre(LSTR("classical"),      32);
                genres.addGenre(LSTR("dance"),          3);
                genres.addGenre(LSTR("easy"),           98);
                genres.addGenre(LSTR("rnb"),            14);
                genres.addGenre(LSTR("jazz"),           8);
                genres.addGenre(LSTR("reggae"),         16);
                genres.addGenre(LSTR("other"),          12);
            }

            inline QString refreshProc(Response * reply, const DataMediaType & /*itemMediaType*/) {
                QJsonObject obj = reply -> toJson();
                return obj.value(LSTR("url")).toString();
            }

            QJsonValue searchProc(const SearchLimit & limits) {
                QString url_str = baseUrlStr(qst_site,
                    QStringLiteral("/search.html?query_search=%1&page=%2")
                        .arg(encodeStr(limits.predicate), OFFSET_TEMPLATE)
                );

                PolyQueryRules rules(
                    call_iter_type_page, limits.start_offset.toInt(),
                    qMin(limits.items_limit, DEFAULT_ITEMS_LIMIT), qMin(limits.requests_limit, 2)
                );
                QueriableResponse response = pRequest(url_str, call_type_html, rules, 0, proc_tracks1);
                return prepareBlock(dmt_audio, cmd_mtd_unknown, response, limits);
            }
        private:
            friend class Singleton<Zaycev>;
            inline Zaycev() {}
            inline virtual ~Zaycev() {}
        };
    }
}

#endif // ZAYCEV
