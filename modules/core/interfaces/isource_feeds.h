#ifndef ISOURCE_FEEDS
#define ISOURCE_FEEDS

#include <qvariant.h>

#include "modules/core/feeder/feed_item.h"

namespace Core {
    class ISourceFeeds {
    public:
        virtual quint64 getFeeds(const FeedType & /*ftypes*/, const quint64 & /*timemark*/, QMap<quint64, FeedItem *> & /*feeds*/) { return QDateTime::currentMSecsSinceEpoch(); }
        virtual QVariant getFeeds(const FeedType & /*ftypes*/, const QVariant & /*limiter*/, QMap<quint64, FeedItem *> & /*feeds*/) { return QVariant(); }
    };
}

#endif // ISOURCE_FEEDS
