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

        struct SearchLimit {
            SearchLimit(const PredicateType & predicate_type, int items_limit, int start_page = 1, int pages_limit = 100) :
                items_limit(items_limit), start_page(start_page), pages_limit(pages_limit), predicate_type(predicate_type) {}

            int items_limit;
            int start_page;
            int pages_limit;
            PredicateType predicate_type;

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
        };

        inline ISearchable() { }
        inline virtual ~ISearchable() {}

        enum ByTypeArg { sets = 1, charts, soundtracks, by_genres, by_years, other, hits, fresh };

        QJsonArray search(const SearchContentType & sc_type, const QString & predicate, const QString & genre, const SearchLimit & limitations) {
            if (!predicate.isEmpty()) {
                if (limitations.by_abc())
                    return byChar(predicate[0], limitations);

                if (limitations.by_type())
                    return byType((ByTypeArg)predicate.mid(0, 1).toInt(), limitations);

                return search_proc(sc_type, predicate, genre, limitations);
            } else if (!genre.isEmpty())
                return byGenre(sc_type, genre, limitations);
            else if (limitations.by_popularity() || predicate.isEmpty())
                return popular(sc_type, genre);

            return QJsonArray();
        }

        virtual QJsonArray byGenre(const QString & /*genre*/, const SearchLimit & /*limitations*/) { return QJsonArray(); }

        virtual QJsonArray byChar(const QChar /*target_char*/, const SearchLimit & /*limitations*/) { return QJsonArray(); }

        virtual QJsonArray byType(const ByTypeArg /*target_type*/, const SearchLimit & /*limitations*/) { return QJsonArray(); }

        virtual QJsonArray popular(const QString & /*genre*/) { return QJsonArray(); }

        virtual QJsonArray related(const QString & /*predicate*/) { return QJsonArray(); }
    protected:
        virtual QJsonArray search_proc(const QString & /*predicate*/, const QString & /*genre*/, const SearchLimit & /*limitations*/) = 0;
    };
}

#endif // ISEARCHABLE
