#include "playlist.h"
#include "../site/requests.h"

using namespace Core::Web::Soundcloud::Api;

QJsonValue Playlist::playlistsByTag(const QString & tag, int count, int offset) {
    return Site::Requests::obj().playlistsByTag(tag. count, offset);
}
