#ifndef ISEARCHABLE
#define ISEARCHABLE

#include <qurl.h>

#include "search_limits.h"
#include "igenreable.h"
#include "modules/core/data_sub_types.h"
#include "modules/core/misc/format.h"

namespace Core {
    class ISearchable : public IGenreable {
    public:
        virtual DataSubType siteType() const = 0;

        inline ISearchable() { }
        inline virtual ~ISearchable() {}

        QJsonValue search(const SearchLimit & limits) {
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

        virtual QJsonValue searchByGenre(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonValue searchByChar(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonValue searchInLyrics(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonValue searchInSets(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonValue newest(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonValue popular(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonValue related(const SearchLimit & /*limits*/) { return QJsonArray(); }
    protected:
        virtual QJsonValue searchProc(const SearchLimit & /*limits*/) = 0;
    };
}

#endif // ISEARCHABLE
