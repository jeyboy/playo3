#ifndef ISEARCHABLE
#define ISEARCHABLE

#include <qurl.h>
#include <qstringlist.h>
#include <qstringbuilder.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

#include "igenreable.h"
#include "modules/core/data_sub_types.h"

namespace Core {
    class ISearchable : public IGenreable {
    public:
        enum SearchContentType {
            sc_audio = 1,
            sc_video = 2,
            sc_all = sc_audio | sc_video
        };
        enum SearchPredicateType {
            in_abc = 1,
            in_title = 2,
            in_artist = 4,
            in_song_name = 8,
            in_tag = 16,
            in_owns = 32,
            in_originals = 64,
            in_mixes = 128,
            in_foreign = 256,
            in_uk_ru = 512,
            in_popular = 1024,
            in_relative = 2048,
            in_lyrics = 4096,
            in_sets = 8192
        };

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

            SearchLimit * updatePredicates(const QString & new_predicate, const QString & new_genre) {
                predicate = new_predicate;
                genre = new_genre;
                return this;
            }

            inline bool include_audio() const { return sc_type & sc_audio; }
            inline bool include_video() const { return sc_type & sc_video; }

            inline bool by_abc() const { return predicate_type & in_abc; }

            inline bool by_artists() const { return predicate_type & in_artist; }
            inline bool by_titles() const { return predicate_type & in_title; }
            inline bool by_songs_name() const { return predicate_type & in_song_name; }
            inline bool by_tags() const { return predicate_type & in_tag; }

            inline bool by_owns() const { return predicate_type & in_owns; }

            inline bool by_originals() const { return predicate_type & in_originals; }
            inline bool by_mixes() const { return predicate_type & in_mixes; }

            inline bool by_foreign() const { return predicate_type & in_foreign; }
            inline bool by_uk_ru() const { return predicate_type & in_uk_ru; }


            inline bool by_popularity() const { return predicate_type & in_popular; }
            inline bool by_relativity() const { return predicate_type & in_relative; }

            inline bool by_lyrics() const { return predicate_type & in_lyrics; }
            inline bool by_sets() const { return predicate_type & in_sets; }

            inline QChar charPredicate() { return predicate.isEmpty() ? QChar('_') : predicate[0]; }
        };

        virtual DataSubType siteType() const = 0;

        inline ISearchable() { }
        inline virtual ~ISearchable() {}

        QJsonArray search(const SearchLimit & limits) {
            if (!limits.predicate.isEmpty()) {
                if (limits.by_abc())
                    return searchByChar(limits);

                if (limits.by_lyrics())
                    return searchInLyrics(limits);

                if (limits.by_sets())
                    return searchInSets(limits);

                if (limits.by_relativity())
                    return related(limits);

                return search_proc(limits);
            } else if (!limits.genre.isEmpty())
                return searchByGenre(limits);
            else
                return popular(limits);
        }

        virtual QJsonArray searchByGenre(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonArray searchByChar(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonArray searchInLyrics(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonArray searchInSets(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonArray popular(const SearchLimit & /*limits*/) { return QJsonArray(); }

        virtual QJsonArray related(const SearchLimit & /*limits*/) { return QJsonArray(); }
    protected:
        virtual QJsonArray search_proc(const SearchLimit & /*limits*/) = 0;
    };
}

#endif // ISEARCHABLE
