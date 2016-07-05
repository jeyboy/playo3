#include "group.h"
#include "../site/requests.h"

using namespace Core::Web::Soundcloud::Api;

QJsonValue Group::groupsByTrack(const QString & track_id, int count, int offset) {
    return Site::Requests::obj().groupsByTrack(track_id, count, offset);
}
