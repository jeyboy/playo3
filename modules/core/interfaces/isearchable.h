#ifndef ISEARCHABLE
#define ISEARCHABLE

#include <qurl.h>

#include "search_limits.h"
#include "igenreable.h"
#include "modules/core/data_sub_types.h"

namespace Core {
    class ISearchable : public IGenreable {
    public:
        virtual DataSubType siteType() const = 0;

        inline ISearchable() { }
        inline virtual ~ISearchable() {}

        QJsonArray search(const SearchLimit & limits) {
            if (limits.by_newest())
                return newest(limits);

            if (limits.by_sets())
                return searchInSets(limits);

            if (!limits.predicate.isEmpty()) {               
                if (limits.by_abc())
                    return searchByChar(limits);

                if (limits.by_lyrics())
                    return searchInLyrics(limits);

                if (limits.in_relative())
                    return related(limits);

                return searchProc(limits);
            } else if (!limits.genre.isEmpty())
                return searchByGenre(limits);
            else
                return popular(limits);
        }

        virtual QJsonArray searchByGenre(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonArray searchByChar(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonArray searchInLyrics(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonArray searchInSets(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonArray newest(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonArray popular(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonArray related(const SearchLimit & /*limits*/) { return QJsonArray(); }
    protected:
        virtual QJsonArray searchProc(const SearchLimit & /*limits*/) = 0;
    };
}

#endif // ISEARCHABLE
