#ifndef SEARCH_LIMITS
#define SEARCH_LIMITS

#include <qvariant.h>
#include <qregularexpression.h>
#include <qstringlist.h>
#include <qstringbuilder.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

#include "icmd_fields.h"
#include "modules/core/web/interfaces/quariable_defines.h"

#define JSON_SEARCH_PREDICATE QStringLiteral("p")
#define JSON_SEARCH_GENRE QStringLiteral("g")

#define JSON_SEARCH_CONTEXT QStringLiteral("s")

#define JSON_SEARCH_ITEMS_LIMIT QStringLiteral("l")
#define JSON_SEARCH_REQUESTS_LIMIT QStringLiteral("r")
#define JSON_SEARCH_OFFSET QStringLiteral("o")

#define JSON_SEARCH_REQUEST_TYPE QStringLiteral("t")
#define JSON_SEARCH_CONTENT_TYPE QStringLiteral("c")
#define JSON_SEARCH_PREDICATE_TYPE QStringLiteral("z")

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
        sp_sets = 8192,
        sp_new = 16384
//        sp_fresh = 32768
    };

    struct SearchLimitBase {
        SearchLimitBase(const SearchContentType & sc_type = sc_all, const SearchPredicateType & predicate_type = sp_popular,
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
        inline bool by_lyrics() const { return predicate_type & sp_lyrics; }
        inline bool by_sets() const { return predicate_type & sp_sets; }
        inline bool by_artists() const { return predicate_type & sp_artist; }
        inline bool by_titles() const { return predicate_type & sp_title; }
        inline bool by_songs_name() const { return predicate_type & sp_song_name; }
        inline bool by_tags() const { return predicate_type & sp_tag; }
        inline bool by_popularity() const { return predicate_type & sp_popular; }
        inline bool by_newest() const { return predicate_type & sp_new; }

        inline bool in_relative() const { return predicate_type & sp_relative; }
        inline bool in_owns() const { return predicate_type & sp_owns; }
        inline bool in_originals() const { return predicate_type & sp_originals; }
        inline bool in_mixes() const { return predicate_type & sp_mixes; }
        inline bool in_foreign() const { return predicate_type & sp_foreign; }
        inline bool in_cyrillic() const { return predicate_type & sp_cyrillic; }
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

        inline QChar charPredicate() { return predicate.isEmpty() ? QChar('_') : predicate[0]; }

        QUrlQuery toICmdParams() {
            QUrlQuery query;

            query.addQueryItem(CMD_MEDIA_TYPE, QString::number(sc_type));
            query.addQueryItem(CMD_PREDICATE_TYPE, QString::number(predicate_type));
            query.addQueryItem(CMD_OFFSET, QString::number(start_offset));
            query.addQueryItem(CMD_ITEMS_LIMIT, QString::number(items_limit));
            query.addQueryItem(CMD_REQUESTS_LIMIT, QString::number(requests_limit));

            if (!predicate.isEmpty())
                query.addQueryItem(CMD_PREDICATE, predicate);

            if (!genre.isEmpty())
                query.addQueryItem(CMD_GENRE, genre);
        }
    };

    struct SearchLimitLayer : public SearchLimit {
        SearchLimitLayer(
            const SearchRequestType & rt, const QVariant & context, const SearchContentType & sc_type,
            const SearchPredicateType & predicate_type, const QString & predicate, const QString & genre,
            int items_limit = DEFAULT_ITEMS_LIMIT, int start_offset = 0, int requests_limit = DEFAULT_REQUESTS_LIMIT) :
            SearchLimit(sc_type, predicate_type, predicate, genre, items_limit, start_offset, requests_limit),
            context(context), req_type(rt) {}

        QVariant context;
        SearchRequestType req_type;

        QString tokenPrefix();
        QString token();
        static void fromJson(const QJsonArray & objs, QList<SearchLimitLayer> & list);
        void save(QJsonArray & arr);
    };

    struct SearchLimitLayers : public SearchLimitBase {
        SearchLimitLayers(const SearchContentType & sc_type = sc_all, const SearchPredicateType & predicate_type = sp_popular,
            int items_limit = DEFAULT_ITEMS_LIMIT, int start_offset = 0, int requests_limit = DEFAULT_REQUESTS_LIMIT) :
            SearchLimitBase(sc_type, predicate_type, items_limit, start_offset, requests_limit) {}

        QStringList tabs;
        QStringList drives;
        QList<int> sites;

        QList<QString> genres;
        QStringList predicates;

        void prepareLayers(QList<SearchLimitLayer> & requests);
    };
}

#endif // SEARCH_LIMITS
