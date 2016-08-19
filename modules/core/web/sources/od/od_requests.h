#ifndef OD_REQUESTS
#define OD_REQUESTS

#include "od_user.h"
#include "od_group.h"
#include "od_artist.h"
#include "od_auth.h"
#include "od_playlist.h"
#include "od_set.h"
#include "od_track.h"
#include "od_video.h"
#include "od_video_playlist.h"

#include "modules/core/web/interfaces/sociable/sociable.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Requests : public Sociable, public Artist, public User, public Auth, public Group,
                    public Playlist, public Set, public Track, public Video, public VideoPlaylist {

                QString regPart() { return tkn_coma_dot % tkn_jsessionid % siteToken(); }

                bool htmlToJson(QueriableArg * arg, Response * reply, QString & /*message*/, bool removeReply) {
                    Html::Document doc = reply -> toHtml(removeReply);

                    bool result = false;

                    switch(arg -> post_proc) {
                        case proc_group1: {
                            Html::Set groups = doc.find(".groups_list li");

                            for(Html::Set::Iterator group = groups.begin(); group != groups.end(); group++) {
                                QJsonObject group_obj;

                                QString id_text = (*group) -> findFirst(".section .o") -> link();

                                group_obj.insert(tkn_perma, id_text.split('/', QString::SkipEmptyParts).first());
                                group_obj.insert(tkn_id, Info::extractLimitedBy(id_text, QStringLiteral("st.groupId="), QStringLiteral("&")));
                                group_obj.insert(tkn_name, (*group) -> findFirst(".caption a") -> text());
                                Html::Tag * img = (*group) -> findFirst(".section img");
                                if (img) {
                                    QString img_url = img -> value(QStringLiteral("src"));

                                    if (img_url.startsWith(QStringLiteral("//")))
                                        img_url = QStringLiteral("http:") % img_url;

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

                                group_obj.insert(tkn_perma, Info::extractLimitedBy(id_text, QStringLiteral("st.referenceName="), QStringLiteral("&")));
                                group_obj.insert(tkn_id, Info::extractLimitedBy(id_text, QStringLiteral("st.groupId="), QStringLiteral("&")));
                                group_obj.insert(tkn_name, name_link -> text());

                                Html::Tag * img = (*group) -> findFirst(".ucard-b_img img");
                                if (img) {
                                    QString img_url = img -> value(QStringLiteral("src"));

                                    if (img_url.startsWith(QStringLiteral("//")))
                                        img_url = QStringLiteral("http:") % img_url;

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

                                group_obj.insert(tkn_id, Info::extractLimitedBy(link, QStringLiteral("st.groupId="), QStringLiteral("&")));
                                group_obj.insert(tkn_perma, link.split('/', QString::SkipEmptyParts).first());
                                group_obj.insert(tkn_name, doc.findFirst(".mctc_name_tx") -> text());
                                Html::Tag * img = doc.findFirst(".add-happening_poster_img");
                                if (img) {
                                    QString img_url = img -> value(QStringLiteral("src"));

                                    if (img_url.startsWith(QStringLiteral("//")))
                                        img_url = QStringLiteral("http:") % img_url;

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

                                user_obj.insert(tkn_id, Info::extractLimitedBy(id_text, QStringLiteral("st.friendId="), QStringLiteral("&")));
                                user_obj.insert(tkn_full_name, name_link -> text());

                                Html::Tag * img = (*user) -> findFirst(".section img");
                                if (img) {
                                    QString img_url = img -> value(QStringLiteral("src"));

                                    if (img_url.startsWith(QStringLiteral("//")))
                                        img_url = QStringLiteral("http:") % img_url;

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
                                    QString img_url = img -> value(QStringLiteral("src"));

                                    if (img_url.startsWith(QStringLiteral("//")))
                                        img_url = QStringLiteral("http:") % img_url;

                                    user_obj.insert(tkn_art_url, img_url);
                                }

                                arg -> append(user_obj, true);
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
                    QJsonObject chunk_obj = response.value(tkn_chunk).toObject();
                    if (chunk_obj.isEmpty()) return false;
                    return chunk_obj.value(tkn_count).toInt() < arg -> per_request_limit/*OD_LIMIT_PER_REQUEST*/;
                }
            protected:
                inline SourceFlags defaultFlags() {
                    return (SourceFlags)(
                        sf_primary_source |
                        /*sf_auth_api_has |*/ sf_auth_site_has | sf_site_online_credentials_req |
                        sf_items_serachable | sf_sets_serachable | sf_users_serachable | sf_groups_serachable |
                        sf_sociable_users | sf_sociable_groups | sf_shareable | sf_packable |
//                        sf_recomendable_by_item | sf_recomendable_by_user |
                        /*sf_newable |*/ sf_populable |

                        /*sf_content_lyrics_has |*/ sf_content_audio_has | sf_content_video_has |
                        sf_content_photo_has | sf_content_news_has |

                        sf_site_auth_mandatory
                    );
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
                    if (siteAdditionalToken().isEmpty()) setSiteAdditionalToken(Auth::grabAdditionalToken());

                    return {{QStringLiteral("TKN"), siteAdditionalToken()}};
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
                            return QUrlQuery((QStringLiteral("gwt.requested=") % siteHash()));
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

//                bool sessionIsValid() { return !Auth::hasError(userInfo().toObject()); }

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

                QJsonValue loadSetData(const QString & attrs) {
                    return tracksByPlaylist(QUrlQuery(attrs).queryItemValue(CMD_ID));
                }



                inline void jsonToUsers(QList<Linkable> & linkables, const QJsonArray & arr) {
                    for(QJsonArray::ConstIterator obj_iter = arr.constBegin(); obj_iter != arr.constEnd(); obj_iter++) {
                        QJsonObject obj = (*obj_iter).toObject();
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

                    QJsonArray arr = usersById(uid).toArray();
                    jsonToUsers(linkables, arr);

                    return linkables;
                }

                QList<Linkable> findGroupsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsByIdOrPerma(uid).toArray();
                    jsonToGroups(linkables, arr);

                    return linkables;
                }

                QList<Linkable> findFriendsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = usersByName(name).toArray();
                    jsonToUsers(linkables, arr);

                    return linkables;
                }

                QList<Linkable> findGroupsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsByName(name).toArray();
                    jsonToGroups(linkables, arr);

                    return linkables;
                }
            public:
                Requests() { setSociableLimitations(true, true, true, true); }

                QString refresh(const QString & item_id, const DataMediaType & item_media_type) {
                    switch(item_media_type) {
                        case dmt_audio: return trackUrl(item_id);
                        case dmt_video: return QString();
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
                        blocks << tracksSearch(limits);

                    if (limits.include_video())
                        blocks << videoSearch(limits);

                    return blocks;
                }

                QJsonValue userInfo() {
                    QJsonObject res = User::userInfo().toObject();

                    clearFriends();
                    jsonToUsers(Friendable::linkables, res.value(tkn_friends).toArray());

                    clearGroups();
                    jsonToGroups(Groupable::linkables, groupsByUser(userID()).toArray());

                    return QJsonArray()
                        << prepareBlock(dmt_audio_set, res.value(tkn_playlists))
                        << prepareBlock(dmt_audio, res.value(tkn_tracks));
                }
            };
        }
    }
}

#endif // OD_REQUESTS
