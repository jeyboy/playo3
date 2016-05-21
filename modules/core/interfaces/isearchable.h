#ifndef ISEARCHABLE
#define ISEARCHABLE

#include <qurl.h>
#include <qstringlist.h>
#include <qstringbuilder.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

#include "igenreable.h"

namespace Core {
    class ISearchable : public IGenreable {
    public:
        enum SearchContentType { sc_audio = 1, sc_video = 2 };
        enum SearchPredicateType { in_abc = 1, in_title = 2, in_artist = 4, in_song = 8, in_tag = 16, in_owns = 32, in_originals = 64, in_foreign = 128, in_popular = 256, in_relative = 512, in_type_arg = 1024 };
        enum ByTypeArg { sets = 1, charts, soundtracks, by_genres, by_years, other, hits, fresh };

        struct SearchLimit {
            SearchLimit(const SearchContentType & sc_type, const SearchPredicateType & predicate_type, const QString & predicate,
                const QString & genre, int items_limit, int start_page = 1, int pages_limit = 100) :
                sc_type(sc_type), predicate_type(predicate_type), predicate(predicate), genre(genre),
                items_limit(items_limit), start_page(start_page), pages_limit(pages_limit) {}

            SearchContentType sc_type;
            SearchPredicateType predicate_type;
            QString predicate;
            QString genre;
            int items_limit;
            int start_page;
            int pages_limit;

            inline bool by_abc() const { return predicate_type & in_abc; }
            inline bool by_artists() const { return predicate_type & in_artist; }
            inline bool by_titles() const { return predicate_type & in_title; }
            inline bool by_songs() const { return predicate_type & in_song; }
            inline bool by_tags() const { return predicate_type & in_tag; }

            inline bool by_popularity() const { return predicate_type & in_popular; }
            inline bool by_owns() const { return predicate_type & in_owns; }
            inline bool by_originals() const { return predicate_type & in_originals; }
            inline bool by_foreign() const { return predicate_type & in_foreign; }
            inline bool by_relativity() const { return predicate_type & in_relative; }
            inline bool by_type() const { return predicate_type & in_type_arg; }

            inline QChar charPredicate() { return predicate.isEmpty() ? QChar('_') : predicate[0]; }
            inline ByTypeArg typePredicate() { return predicate.isEmpty() ? other : (ByTypeArg)predicate.mid(0, 1).toInt(); }
        };

        inline ISearchable() { }
        inline virtual ~ISearchable() {}

        QJsonArray search(const SearchLimit & limits) {
            if (!limits.predicate.isEmpty()) {
                if (limits.by_abc())
                    return searchByChar(limits);

                if (limits.by_type())
                    return searchByType(limits);

                return search_proc(limits);
            } else if (!limits.genre.isEmpty())
                return searchByGenre(limits);
            else if (limits.by_popularity() || limits.predicate.isEmpty())
                return popular(limits);

            return QJsonArray();
        }

        virtual QJsonArray searchByGenre(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonArray searchByChar(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonArray searchByType(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonArray popular(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonArray related(const SearchLimit & /*limits*/) { return QJsonArray(); }
    protected:
        virtual QJsonArray search_proc(const SearchLimit & /*limits*/) = 0;
    };
}

#endif // ISEARCHABLE
