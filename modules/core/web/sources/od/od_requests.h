#ifndef OD_REQUESTS
#define OD_REQUESTS

#include "od_layer.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Requests : public Layer {

                QString regPart() { return tkn_coma_dot % tkn_jsessionid % siteToken(); }

                bool htmlToJson(QueriableArg * arg, Response * reply, QString & message, bool removeReply) {
                    Html::Document doc = reply -> toHtml(removeReply);

                    bool result = false;

                    switch(arg -> post_proc) {
                        case proc_group1: {
                            Html::Set groups = doc.find(".groups_list li");

                            for(Html::Set::Iterator group = groups.begin(); group != groups.end(); group++) {
                                QJsonObject group_obj;

                                QString id_text = (*group) -> findFirst(".section .o") -> link();

                                group_obj.insert(tkn_perma, id_text.split('/', QString::SkipEmptyParts).first());
                                group_obj.insert(tkn_id, Info::extractLimitedBy(id_text, LSTR("st.groupId="), LSTR("&")));
                                group_obj.insert(tkn_name, (*group) -> findFirst(".caption a") -> text());
                                Html::Tag * img = (*group) -> findFirst(".section img");
                                if (img) {
                                    QString img_url = img -> src();

                                    if (img_url.startsWith(LSTR("//")))
                                        img_url = LSTR("http:") % img_url;

                                    group_obj.insert(tkn_art_url, img_url);
                                }

                                arg -> append(group_obj, group + 1 == groups.end());
                            }

                            result = !groups.isEmpty();
                        break;}

                        case proc_group2: {
                            Html::Set groups = doc.find(".gs_result_group-card");

                            for(Html::Set::Iterator group = groups.begin(); group != groups.end(); group++) {
                                QJsonObject group_obj;

                                Html::Tag * name_link = (*group) -> findFirst(".gs_result_i_t_name");

                                QString id_text = name_link -> link();

                                group_obj.insert(tkn_perma, Info::extractLimitedBy(id_text, LSTR("st.referenceName="), LSTR("&")));
                                group_obj.insert(tkn_id, Info::extractLimitedBy(id_text, LSTR("st.groupId="), LSTR("&")));
                                group_obj.insert(tkn_name, name_link -> text());

                                Html::Tag * img = (*group) -> findFirst(".ucard-b_img img");
                                if (img) {
                                    QString img_url = img -> src();

                                    if (img_url.startsWith(LSTR("//")))
                                        img_url = LSTR("http:") % img_url;

                                    group_obj.insert(tkn_art_url, img_url);
                                }

                                arg -> append(group_obj, group + 1 == groups.end());
                            }

                            result = !groups.isEmpty();
                        break;}

                        case proc_group3: {
                            result = !doc.findFirst(".p404");

                            if (result) {
                                QJsonObject group_obj;

                                QString link = doc.findFirst(".media-text_a") -> link();

                                group_obj.insert(tkn_id, Info::extractLimitedBy(link, LSTR("st.groupId="), LSTR("&")));
                                group_obj.insert(tkn_perma, link.split('/', QString::SkipEmptyParts).first());
                                group_obj.insert(tkn_name, doc.findFirst(".mctc_name_tx") -> text());
                                Html::Tag * img = doc.findFirst(".add-happening_poster_img");
                                if (img) {
                                    QString img_url = img -> src();

                                    if (img_url.startsWith(LSTR("//")))
                                        img_url = LSTR("http:") % img_url;

                                    group_obj.insert(tkn_art_url, img_url);
                                }

                                arg -> append(group_obj, true);
                            }
                        break;}

                        case proc_user1: {
                            Html::Set users = doc.find(".gs_result_i_w");

                            for(Html::Set::Iterator user = users.begin(); user != users.end(); user++) {
                                QJsonObject user_obj;

                                Html::Tag * name_link = (*user) -> findFirst(".caption a");

                                QString id_text = name_link -> link(); // link also contains perma

                                user_obj.insert(tkn_id, Info::extractLimitedBy(id_text, LSTR("st.friendId="), LSTR("&")));
                                user_obj.insert(tkn_full_name, name_link -> text());

                                Html::Tag * img = (*user) -> findFirst(".section img");
                                if (img) {
                                    QString img_url = img -> src();

                                    if (img_url.startsWith(LSTR("//")))
                                        img_url = LSTR("http:") % img_url;

                                    user_obj.insert(tkn_art_url, img_url);
                                }

                                arg -> append(user_obj, user + 1 == users.end());
                            }

                            result = !users.isEmpty();
                        break;}

                        case proc_user2: {
                            result = !doc.findFirst(".p404");

                            if (result) {
                                QJsonObject user_obj;

                                QString link = doc.findFirst(".media-text_a") -> link();

                                user_obj.insert(tkn_id, link.split('/', QString::SkipEmptyParts)[1]);
                                user_obj.insert(tkn_full_name, doc.findFirst(".mctc_name_tx") -> text());
                                Html::Tag * img = doc.findFirst("#viewImageLinkId");
                                if (img) {
                                    QString img_url = img -> src();

                                    if (img_url.startsWith(LSTR("//")))
                                        img_url = LSTR("http:") % img_url;

                                    user_obj.insert(tkn_art_url, img_url);
                                }

                                arg -> append(user_obj, true);
                            }
                        break;}

                        case proc_video1: {
                            ///////// extract and apply token ///////////
                            Html::Tag * container = doc.findFirst(".loader-container");

                            if (container) {
                                QString id = container -> data(LSTR("last-element"));

                                if (id.isEmpty())
                                   qDebug() << "OD: token is missed for token based requests";
                                else
                                    arg -> prepareRequestUrlByToken(id);
                            } else {
                                if (!doc.findFirst("#vv_main_content"))
                                    message = LSTR("OD: video loader is missed");
                                else
                                    message = LSTR("OD: do not have any content");
                            }
                            ////////////////////////////////////////////

                            Html::Set video = doc.find(".vid-card");

                            for(Html::Set::Iterator vid = video.begin(); vid != video.end(); vid++) {
                                QJsonObject vid_obj;

                                QString oid = (*vid) -> data(tkn_id);

                                if (oid.isEmpty()) { // if id is missed - then obj is set
                                    QString album_id = (*vid) -> findFirst(".vid-card_cnt") -> data(tkn_id);

                                    vid_obj.insert(
                                        tkn_loadable_cmd,
                                        Cmd::build(sourceType(), cmd_mtd_video_by_playlist, {{CMD_ID, album_id}}).toString()
                                    );
                                } else {
                                    vid_obj.insert(tkn_id, oid);
                                    vid_obj.insert(tkn_duration, Duration::toSecs((*vid) -> findFirst(".vid-card_duration") -> text()));
                                }

                                QString img_url = (*vid) -> findFirst("img") -> src();
                                if (img_url.startsWith(LSTR("//")))
                                    img_url = LSTR("http:") % img_url;
                                vid_obj.insert(tkn_art_url, img_url);

                                vid_obj.insert(tkn_name, (*vid) -> findFirst(".vid-card_n") -> text());

                                arg -> append(vid_obj, vid + 1 == video.end());
                            }

                            result = !video.isEmpty();
                        break;}

                        case proc_video2: {
                            Html::Tag * video_box = doc.findFirst(".vp_video .vid-card_cnt");

                            if ((result = !!video_box))
                                arg -> append(videoVariants(video_box).toArray());
                        break;}

                        case proc_video3: {
                            Html::Set video = doc.find(".video-card");

                            for(Html::Set::Iterator vid = video.begin(); vid != video.end(); vid++) {
//                                qDebug() << (*(*vid));

                                if (!(*vid) -> hasClass(LSTR("__channel"))) { // ignore channels
                                    QJsonObject vid_obj;

                                    vid_obj.insert(tkn_id, (*vid) -> data(tkn_id));

                                    vid_obj.insert(tkn_duration, Duration::toSecs((*vid) -> findFirst(".video-card_duration") -> text()));

                                    QString img_url = (*vid) -> findFirst("img") -> src();
                                    if (img_url.startsWith(LSTR("//")))
                                        img_url = LSTR("http:") % img_url;
                                    vid_obj.insert(tkn_art_url, img_url);

                                    vid_obj.insert(tkn_name, (*vid) -> findFirst(".video-card_n") -> text());
                                    arg -> append(vid_obj, vid + 1 == video.end());
                                }
                            }

                            result = !video.isEmpty();
                        break;}

                        case proc_video4: {
                            Html::Tag * video_box = doc.findFirst(".vp-layer_cnt");

                            if ((result = !!video_box)) {
                                QJsonObject obj;
                                obj.insert(tkn_grab_url, videoVariants(video_box -> findFirst(".vid-card_cnt")));

                                obj.insert(tkn_name, video_box -> findFirst(".vp-layer-info .textWrap") -> text());

                                obj.insert(tkn_id, video_box -> findFirst("#movie-comments") -> data("subjectid"));
                                obj.insert(tkn_duration, Duration::toSecs(video_box -> findFirst(".vid-card_duration") -> text()));

                                QString img_url = video_box -> findFirst(".vid-card_img") -> src();
                                if (img_url.startsWith(LSTR("//")))
                                    img_url = LSTR("http:") % img_url;
                                obj.insert(tkn_art_url, img_url);

                                arg -> append(obj);
                            }
                        break;}

                        default: ;
                    }

                    return result;
                }
                bool extractStatus(QueriableArg * arg, QJsonObject & json, int & code, QString & message) {
                    if (Auth::retryRequired(json, message)) {
                        QString predicate = regPart();
                        if (takeOnlineCredentials())
                            arg -> updateUrls(predicate, regPart());

                        return false;
                    }

                    if (!message.isEmpty()) {
                        code = -1;
                        return false;
                    }

                    return true;
                }
                inline bool endReached(QJsonObject & response, QueriableArg * arg) {
                    QJsonObject chunk_obj = JSON_OBJ(response, tkn_chunk);
                    if (chunk_obj.isEmpty()) return false;
                    return JSON_INT(chunk_obj, tkn_count) < arg -> per_request_limit;
                }
            protected:
                Requests() {
                    setSociableLimitations(true, true, true, true);

                    setSiteLocale(LSTR("ru")); // INFO: temp

                    flags = QHash<SourceFlags, SourceFlags>{
                        {sf_endpoint, (SourceFlags)(
                            sf_online_credentials_req | sf_is_primary | sf_audio | sf_video | sf_user/*sf_sociable*/ |
                            sf_photo | sf_feed | sf_playlist | sf_compilation | sf_site | sf_site_connectable)
                        },

                        {sf_feed,                   sf_site},

                        {sf_feed_by_user,           sf_site_auth_only},
                        {sf_feed_by_group,          sf_site_auth_only},

                        {sf_is_shareable,           sf_site_auth_only},

                        {sf_search,                 sf_site_auth_only},

                        {sf_compilation,            sf_site_auth_only},

                        {sf_stream_list,            sf_site_auth_only},

                        {sf_album_by_title,         sf_site_auth_only},

                        {sf_artist_by_id,           sf_site_auth_only},
                        {sf_artist_by_title,        sf_site_auth_only},

                        {sf_popular_artist,         sf_site_auth_only},

                        {sf_popular_audio,          sf_site_auth_only},
                        {sf_audio_by_id,            sf_site_auth_only},
                        {sf_audio_by_title,         sf_site_auth_only},
                        {sf_audio_by_stream,        sf_site_auth_only},
                        {sf_audio_by_artist,        sf_site_auth_only},
                        {sf_audio_by_compilation,   sf_site_auth_only},
                        {sf_audio_by_playlist,      sf_site_auth_only},
                        {sf_audio_by_album,         sf_site_auth_only},
                        {sf_audio_by_user,          sf_site_auth_only},
                        {sf_audio_recs_by_user,     sf_site_auth_only},
//                        {sf_audio_recs_by_track,    sf_site_auth_only},

                        {sf_audio_playlist_by_id,   sf_site_auth_only},

                        {sf_video_by_id,            sf_site_auth_only},
                        {sf_video_by_title,         sf_site_auth_only},
                        {sf_video_by_user,          sf_site_auth_only},
                        {sf_video_by_category,      sf_site_auth_only},
                        {sf_video_by_playlist,      sf_site_auth_only},


                        {sf_user_sociable,          sf_site_auth_only},
                        {sf_user_by_id,             sf_site_auth_only},
                        {sf_user_by_title,          sf_site_auth_only},

//                        {sf_group_sociable,         sf_site_auth_only}, // not finished
                        {sf_group_by_user,          sf_site_auth_only},
                        {sf_group_by_title,         sf_site_auth_only},
                        {sf_group_by_id,            sf_site_auth_only},
                    };
                }

                void saveAdditionals(QJsonObject & obj) {
                    setSiteHash(QString());
                    setSiteToken(QString()); // drop old token
                    setSiteAdditionalToken(QString()); // drop old add token

                    Sociable::toJson(obj);
                    Manager::saveCookies(obj, QUrl(url_root));
                }
                void loadAdditionals(QJsonObject & obj) {
                    Sociable::fromJson(obj);
                    Manager::loadCookies(obj);
                }
                void clearAdditionals() {
                    clearFriends();
                    clearGroups();
                    Manager::removeCookies(QUrl(url_root));
                }

                inline Headers tknHeaders() {
                    if (siteAdditionalToken().isEmpty())
                        initSession();

                    setSiteAdditionalToken(Auth::grabNewAdditionalToken()); //FIXME need to update token after 1 minute interval only - not each request

                    return {{tkn_header, siteAdditionalToken()}};
                }

                inline QUrlQuery genDefaultParams(const QuerySourceType & stype) {
                    switch(stype) {
                        case qst_site:
                        case qst_site_audio: return QUrlQuery();
                        default: {
                            if (siteHash().isEmpty()) {
                                QString add_token;
                                setSiteHash(Auth::grabHash(add_token));
                                setSiteAdditionalToken(add_token);
                            }
                            return QUrlQuery((LSTR("gwt.requested=") % siteHash()));
                        }
                    }
                }

                inline QString baseUrlStr(const QuerySourceType & stype, const QString & predicate) {
                    switch(stype) {
                        case qst_site_audio: return url_base_audio % predicate;
                        case qst_site: return url_root % predicate;
                        default: return url_root % predicate;
                    }
                }
                QString baseUrlStr(const QuerySourceType & stype, const QString & predicate, const QUrlQuery & query) {
                    switch(stype) {
                        case qst_site_audio: {
                            if (siteToken().isEmpty())
                                takeOnlineCredentials();

                            return Auth::baseUrlStr(stype, predicate % regPart(), query);
                        }

                        default: return Auth::baseUrlStr(stype, predicate, query);
                    }
                }

                bool connectUserSite() {
                    QString user_id;

                    if (siteConnection(user_id, error)) {
                        setSiteUserID(user_id);

                        return true;
                    }
                    else return false;
                }

                bool takeOnlineCredentials() {
                    QString sid = Auth::grabSID();

                    if (sid.isEmpty()) {
                        disconnectUser();
                        return false;
                    } else {
                        setSiteToken(sid);
                        return true;
                    }
//                    return sessionIsValid();
                }

                QJsonValue loadContainerData(const QString & attrs) {
                    return audioByPlaylist(QUrlQuery(attrs).queryItemValue(CMD_ID));
                }

                inline void jsonToUsers(QList<Linkable> & linkables, const QJsonArray & arr) {
                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
                        QJsonObject obj = (*obj_iter).toObject();

                        if (JSON_HAS_KEY(obj, Web::tkn_error)) continue;

                        linkables << Linkable(
                            obj.value(tkn_id).toString(),
                            obj.value(tkn_full_name).toString(),
                            QString(),
                            obj.value(tkn_art_url).toString()
                        );
                    }
                }
                inline void jsonToGroups(QList<Linkable> & linkables, const QJsonArray & arr) {
                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
                        QJsonObject obj = (*obj_iter).toObject();

                        if (JSON_HAS_KEY(obj, Web::tkn_error)) continue;

                        linkables << Linkable(
                            obj.value(tkn_id).toString(),
                            obj.value(tkn_name).toString(),
                            obj.value(tkn_perma).toString(),
                            obj.value(tkn_art_url).toString()
                        );
                    }
                }

                QList<Linkable> findFriendsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonObject friends_res = usersById(uid).toObject();
                    jsonToUsers(linkables, EXTRACT_ITEMS(friends_res));

                    return linkables;
                }

                QList<Linkable> findGroupsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonObject groups_res = groupsByIdOrPerma(uid).toObject();
                    jsonToGroups(linkables, EXTRACT_ITEMS(groups_res));

                    return linkables;
                }

                QList<Linkable> findFriendsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonObject friends_res = usersByName(name).toObject();
                    jsonToUsers(linkables, EXTRACT_ITEMS(friends_res));

                    return linkables;
                }

                QList<Linkable> findGroupsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonObject groups_res = groupsByName(name).toObject();
                    jsonToGroups(linkables, EXTRACT_ITEMS(groups_res));

                    return linkables;
                }
            public:
                QString refresh(const QString & item_id, const DataMediaType & item_media_type) {
                    switch(item_media_type) {
                        case dmt_audio: return audioUrl(item_id);
                        case dmt_video: return videoUrl(item_id);
                        default:;
                    }

                    return QString();
                }

                QJsonValue popular(const SearchLimit & limits) {
                    QJsonArray blocks;

                    if (limits.include_audio())
                        blocks << setByType(set_popular_tracks, limits);

                    return blocks;
                }

                // TODO: not logged in error
                QJsonValue searchProc(const SearchLimit & limits) {
                    QJsonArray blocks;

                    if (limits.include_audio())
                        blocks << audioSearch(limits);

                    if (limits.include_video())
                        blocks << videoSearch(limits);

                    return blocks;
                }

                QJsonValue userInfo(const QString & user_id) {
                    if (user_id == userID()) {
                        QJsonObject res = User::userInfo().toObject();

                        clearFriends();
                        jsonToUsers(Friendable::linkables, res.value(tkn_friends).toArray());

                        clearGroups();
                        QJsonObject groups_res = groupsByUser(userID()).toObject();
                        jsonToGroups(Groupable::linkables, EXTRACT_ITEMS(groups_res));

                        return QJsonArray()
                            << prepareBlock(dmt_audio_set, res.value(tkn_playlists))
                            << prepareBlock(dmt_audio, res.value(tkn_tracks));
                    }
                    else return userMedia(user_id);
                }

                QJsonValue userMedia(const QString & user_id) {
                    QJsonObject response = User::userMedia(user_id).toObject();
                    QJsonArray blocks;

                    QJsonArray artists = response.value(tkn_artists).toArray();
                    if (!artists.isEmpty()) {
                        Artist::prepareArtists(artists);
                        blocks << prepareBlock(dmt_audio_set, artists, {{tkn_dir_name, LSTR("Artists")}});
                    }

                    QJsonArray albums = response.value(tkn_albums).toArray();
                    if (!albums.isEmpty()) {
                        Album::prepareAlbums(albums);
                        blocks << prepareBlock(dmt_audio_set, albums, {{tkn_dir_name, LSTR("Albums")}});
                    }

                    blocks << prepareBlock(dmt_audio_set, response.value(tkn_playlists));
                    blocks << prepareBlock(dmt_audio, response.value(tkn_tracks));

                    return blocks;
                }
            };
        }
    }
}

#endif // OD_REQUESTS
