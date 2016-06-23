#ifndef FEED_ITEM
#define FEED_ITEM

#include <qdatetime.h>
#include <qurl.h>

enum FeedType : int {
    feed_other = 0,
    feed_audio = 1,
    feed_video = 2,
    feed_post = 4,
    feed_photo = 8,

    feed_media = feed_audio | feed_video,
    feed_info = feed_post | feed_photo,
    feed_all = feed_other | feed_media | feed_info
};

struct FeedItem {
    FeedType ftype;
    qint64 timestamp;
    QString title;
    QUrl link;
    QList<QString> images;
    QString desc;
};

#endif // FEED_ITEM
