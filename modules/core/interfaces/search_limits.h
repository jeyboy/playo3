#ifndef SEARCH_LIMITS
#define SEARCH_LIMITS

#include <qstringlist.h>
#include <qstringbuilder.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

#include "modules/core/web/interfaces/quariable_defines.h"

#define JSON_SEARCH_PREDICATE QStringLiteral("p")
#define JSON_SEARCH_GENRE QStringLiteral("g")
#define JSON_SEARCH_SUBJECT QStringLiteral("s")
#define JSON_SEARCH_LIMIT QStringLiteral("l")
#define JSON_SEARCH_REQUEST_TYPE QStringLiteral("t")
#define JSON_SEARCH_CONTENT_LIMIT QStringLiteral("c")

namespace Core {
    enum SearchRequestType { sr_local, sr_inner, sr_remote };

    enum SearchContentType {
        sc_audio = 1,
        sc_video = 2,
        sc_all = sc_audio | sc_video
    };
    enum SearchPredicateType {
        sp_abc = 1,
        sp_title = 2,
        sp_artist = 4,
        sp_song_name = 8,
        sp_tag = 16,
        sp_owns = 32,
        sp_originals = 64,
        sp_mixes = 128,
        sp_foreign = 256,
        sp_cyrillic = 512,
        sp_popular = 1024,
        sp_relative = 2048,
        sp_lyrics = 4096,
        sp_sets = 8192
    };

    struct SearchLimitBase {
        SearchLimitBase(const SearchContentType & sc_type, const SearchPredicateType & predicate_type,
            int items_limit = DEFAULT_ITEMS_LIMIT, int start_offset = 0, int requests_limit = DEFAULT_REQUESTS_LIMIT) :
            sc_type(sc_type), predicate_type(predicate_type), items_limit(items_limit), start_offset(start_offset),
            requests_limit(requests_limit) {}

        SearchContentType sc_type;
        SearchPredicateType predicate_type;

        int items_limit;
        int start_offset;
        int requests_limit;

        inline bool include_audio() const { return sc_type & sc_audio; }
        inline bool include_video() const { return sc_type & sc_video; }

        inline bool by_abc() const { return predicate_type & sp_abc; }
        inline bool by_popularity() const { return predicate_type & sp_popular; }
        inline bool by_relativity() const { return predicate_type & sp_relative; }
        inline bool by_lyrics() const { return predicate_type & sp_lyrics; }
        inline bool by_sets() const { return predicate_type & sp_sets; }

        inline bool by_artists() const { return predicate_type & sp_artist; }
        inline bool by_titles() const { return predicate_type & sp_title; }
        inline bool by_songs_name() const { return predicate_type & sp_song_name; }
        inline bool by_tags() const { return predicate_type & sp_tag; }
        inline bool by_owns() const { return predicate_type & sp_owns; }
        inline bool by_originals() const { return predicate_type & sp_originals; }
        inline bool by_mixes() const { return predicate_type & sp_mixes; }
        inline bool by_foreign() const { return predicate_type & sp_foreign; }
        inline bool by_cyrillic() const { return predicate_type & sp_cyrillic; }

        inline QChar charPredicate() { return predicate.isEmpty() ? QChar('_') : predicate[0]; }
    };


    struct SearchLimit : SearchLimitBase {
        SearchLimit(
            const SearchContentType & sc_type, const SearchPredicateType & predicate_type,
            const QString & predicate, const QString & genre, int items_limit = DEFAULT_ITEMS_LIMIT,
            int start_offset = 0, int requests_limit = DEFAULT_REQUESTS_LIMIT) :
            SearchLimitBase(sc_type, predicate_type, items_limit, start_offset, requests_limit),
            predicate(predicate), genre(genre) {}

        QString predicate;
        QString genre;

//        SearchLimit * updatePredicates(const QString & new_predicate, const QString & new_genre) {
//            predicate = new_predicate;
//            genre = new_genre;
//            return this;
//        }

        QString token();
        static void fromJson(const QJsonArray & objs, QList<SearchLimit> & list);
        void save(QJsonArray & arr);
    };


    struct SearchLimitLayers : public SearchLimitBase {
        QStringList tabs;
        QStringList drives;
        QList<int> sites;

        QList<QString> genres;
        QStringList predicates;

        void prepareLayers(QList<SearchLimitLayer> & requests) {
            bool web_predicable = !(request.predicates.isEmpty() && request.genres.isEmpty()) || request.popular;

            if (request.predicates.isEmpty()) request.predicates << QString();
            if (request.genres.isEmpty()) request.genres << QString();

            for(QStringList::Iterator it = request.predicates.begin(); it != request.predicates.end(); it++) {
                QString predicate = (*it).replace(QRegularExpression("['\"]"), " ");

                for(QList<QString>::Iterator genre_it = request.genres.begin(); genre_it != request.genres.end(); genre_it++) {
                    if (web_predicable && request.inSites)
                        for(QList<void *>::Iterator search_interface = request.sites.begin(); search_interface != request.sites.end(); search_interface++)
                            requests.append(SearchRequest(SearchRequest::remote, (*search_interface), request.ctype, predicate, (*genre_it), request.popular));

                    if (request.inTabs) {
                        for(QList<void *>::Iterator tab = request.tabs.begin(); tab != request.tabs.end(); tab++)
                            requests.append(SearchRequest(SearchRequest::inner, (*tab), request.ctype, predicate, (*genre_it), request.popular));
                    }

                    if (request.inComputer)
                        for(QStringList::Iterator drive = request.drives.begin(); drive != request.drives.end(); drive++)
                            requests.append(SearchRequest(SearchRequest::local, new QString(*drive), request.ctype, predicate, (*genre_it), request.popular));
                }
            }
        }
    };


    struct SearchLimitLayer : public SearchLimit {
        SearchLimitLayer(
            const SearchRequestType & rt, const QString & subject, const SearchContentType & sc_type,
            const SearchPredicateType & predicate_type, const QString & predicate, const QString & genre,
            int items_limit = DEFAULT_ITEMS_LIMIT, int start_offset = 0, int requests_limit = DEFAULT_REQUESTS_LIMIT) :
            SearchLimit(sc_type, predicate_type, predicate, genre, items_limit, start_offset, requests_limit),
            context(subject), req_type(rt) {}

        QString context;
        SearchRequestType req_type;
    };
}

#endif // SEARCH_LIMITS
