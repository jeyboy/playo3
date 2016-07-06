#ifndef SOUNDCLOUD_API_H
#define SOUNDCLOUD_API_H

#include "modules/core/interfaces/singleton.h"
#include "soundcloud/soundcloud_requests.h"

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
                        groupInfo(oid.mid(1), res);
                    else
                        userInfo(oid, res);

                    return res;
                }
                inline void objectInfoAsync(const QString & oid, Func * func) {
                    ThreadUtils::obj().run(this, &Queries::objectInfo, oid, func);
                }


                void groupInfo(const QString & group_id, QJsonObject & object) {
                    object.insert(tkn_audio_list, tracksByGroup(group_id));
                }

                void userInfo(const QString & user_id, QJsonObject & object) {
                    object.insert(tkn_audio_list, tracksByUser(user_id));
                    object.insert(tkn_playlist, playlistsByUser(user_id));
                    QThread::msleep(REQUEST_DELAY);
                    object.insert(tkn_followings, userFollowings(user_id)); // return bad request error
                    object.insert(tkn_followers, userFollowers(user_id));
                    QThread::msleep(REQUEST_DELAY);
                    object.insert(tkn_groups, groupsByUser(user_id));
                }


                QString refresh(const QString & track_id, const DataMediaType & /*itemMediaType*/) {
                    return trackUrl(track_id);
                }

                QList<Linkable> findFriendsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = usersById(uid).toArray();
                    jsonToUsers(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findFriendsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = usersByName(name, 400).toArray();
                    jsonToUsers(linkables, arr);

                    return linkables;
                }

                QList<Linkable> findGroupsById(const QString & uid) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsById(uid).toArray();
                    jsonToGroups(linkables, arr);

                    return linkables;
                }
                QList<Linkable> findGroupsByName(const QString & name) {
                    QList<Linkable> linkables;

                    QJsonArray arr = groupsByName(name, 400).toArray();
                    jsonToGroups(linkables, arr);

                    return linkables;
                }

            protected:
                QJsonValue searchInSets(const SearchLimit & limits) {
                    QString predicate = predicate.isEmpty() ? limits.genre : limits.predicate;
                    return playlistsByPredicate(predicate, limits.items_limit, limits.start_offset);
                }

                QJsonValue searchProc(const SearchLimit & limits) { return tracksSearch(limits); }
            };
        }
    }
}

#endif // SOUNDCLOUD_API_H
