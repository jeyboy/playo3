#include "group.h"
#include "../site/requests.h"

using namespace Core::Web::Soundcloud::Api;

QJsonValue Group::groupsByTrack(const QString & track_id, int count = SOUNDCLOUD_ITEMS_LIMIT, int offset = 0) {
    return Site::Requests::obj().groupsByTrack(track_id, count, offset);
}
