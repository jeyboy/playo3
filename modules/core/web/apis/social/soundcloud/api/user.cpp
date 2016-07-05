#include "user.h"

#include "../site/requests.h"

using namespace Core::Web::Soundcloud::Api;

QJsonValue User::usersByTrackReposting(const QString & track_id, int count, int offset) {
    return Site::Requests::obj().usersByTrackReposting(track_id, count, offset);
}
