#include "group.h"
#include "../site/group.h"


using namespace Core::Web::Soundcloud::Api;

QJsonValue Group::groupsByTrack(const QString & /*user_id*/, int /*count*/ = SOUNDCLOUD_ITEMS_LIMIT, int /*offset*/ = 0) {
    Core::Web::Soundcloud::Site::Group group;
    return group.groupsByTrack()
}
