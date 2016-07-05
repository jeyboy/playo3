#ifndef SOUNDCLOUD_API_H
#define SOUNDCLOUD_API_H

#include "modules/core/interfaces/singleton.h"
#include "soundcloud/requests.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Queries : public Requests, public Singleton<Queries> {
                Q_OBJECT

                friend class Singleton<Queries>;
                inline Queries() { setSearchLimitations(true, true, true, true); }
            public:
                inline QString name() const { return val_name; }
                inline DataSubType siteType() const { return dt_site_sc; }

                QJsonObject objectInfo(const QString & oid) {
                    QJsonObject res;

                    if (oid[0] == '-')
                        getGroupInfo(oid.mid(1), res);
                    else
                        getUserInfo(oid, res);

                    return res;
                }
                inline void objectInfoAsync(const QString & oid, Func * func) {
                    ThreadUtils::obj().run((Requests *)this, &Requests::objectInfo, oid, func);
                }


                void groupInfo(const QString & group_id, QJsonObject & object) {
                    object.insert(tkn_audio_list, tracksByGroup(group_id));
                }

                void userInfo(const QString & user_id, QJsonObject & object) {
                    object.insert(tkn_audio_list, userAudio(user_id));
                    object.insert(tkn_playlist, userPlaylists(user_id));
                    QThread::msleep(REQUEST_DELAY);
                    object.insert(tkn_followings, userFollowings(user_id)); // return bad request error
                    object.insert(tkn_followers, userFollowers(user_id));
                    QThread::msleep(REQUEST_DELAY);
                    object.insert(tkn_groups, userGroups(user_id));
                }


                QString refresh(const QString & audio_uid, const DataMediaType & /*itemMediaType*/) {
                    return idToPathSite(audio_uid);
                }

                QList<Linkable> findFriendsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = userById(uid);
                    jsonToUsers(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findFriendsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = usersByName(name, 400);
                    jsonToUsers(linkables, arr);

                    return linkables;
                }

                QList<Linkable> findGroupsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupById(uid);
                    jsonToGroups(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findGroupsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsByName(name, 400);
                    jsonToGroups(linkables, arr);

                    return linkables;
                }

            protected:
                QJsonValue searchInSets(const SearchLimit & limits) {
                    QString predicate = predicate.isEmpty() ? limits.genre : limits.predicate;
                    return playlistByPredicate(predicate, limits.items_limit, limits.start_offset);
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_API_H
