#ifndef ISOURCE_FEEDS
#define ISOURCE_FEEDS

#include <qvariant.h>

#include "modules/core/feeder/feed_item.h"

#define FEED_ATTR_TYPE QStringLiteral("ftype")
#define FEED_ATTR_GENRE QStringLiteral("fgenre")

namespace Core {
    class ISourceFeeds {
    public:
        virtual quint64 getFeeds(const QVariantMap & /*attrs*/, const quint64 & /*timemark*/, QMap<quint64, FeedItem *> & /*feeds*/) { return QDateTime::currentMSecsSinceEpoch(); }
        virtual QVariant getFeeds(const QVariantMap & /*attrs*/, const QVariant & /*limiter*/, QMap<quint64, FeedItem *> & /*feeds*/) { return QVariant(); }
    };
}

#endif // ISOURCE_FEEDS
