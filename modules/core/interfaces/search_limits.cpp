#include "search_limits.h"

using namespace Core;

void SearchLimitLayers::prepareLayers(QList<SearchLimitLayer> & requests) {
    bool web_predicable = !(predicates.isEmpty() && genres.isEmpty()) || by_popularity() || by_newest() || by_sets();

    if (predicates.isEmpty()) predicates << QString();
    if (genres.isEmpty()) genres << QString();

    for(QStringList::Iterator it = predicates.begin(); it != predicates.end(); it++) {
        QString predicate = (*it).replace(QRegularExpression("['\"]"), " ");

        for(QList<QString>::Iterator genre_it = genres.begin(); genre_it != genres.end(); genre_it++) {
            if (web_predicable && !sites.isEmpty())
                for(QList<int>::Iterator site = sites.begin(); site != sites.end(); site++)
                    requests.append(
                        SearchLimitLayer(
                            sr_remote,
                            *site,
                            sc_type,
                            predicate_type,
                            predicate,
                            *genre_it,
                            items_limit,
                            start_offset,
                            requests_limit
                        )
                    );

            if (!tabs.isEmpty()) {
                for(QStringList::Iterator tab = tabs.begin(); tab != tabs.end(); tab++)
                    requests.append(
                        SearchLimitLayer(
                            sr_inner,
                            *tab,
                            sc_type,
                            predicate_type,
                            predicate,
                            *genre_it,
                            items_limit,
                            start_offset,
                            requests_limit
                        )
                    );
            }

            if (!drives.isEmpty())
                for(QStringList::Iterator drive = drives.begin(); drive != drives.end(); drive++)
                    requests.append(
                        SearchLimitLayer(
                            sr_local,
                            *drive,
                            sc_type,
                            predicate_type,
                            predicate,
                            *genre_it,
                            items_limit,
                            start_offset,
                            requests_limit
                        )
                    );
        }
    }
}

QString SearchLimitLayer::tokenPrefix() {
    QString prefix = QStringLiteral("By titles: ");

    if (by_artists())
        prefix = QStringLiteral("By artists: ");
    else if (by_songs_name())
        prefix = QStringLiteral("By songs: ");
    else if (by_sets())
        prefix = QStringLiteral("By sets: ");
    else if (by_abc())
        prefix = QStringLiteral("By abc: ");
    else if (by_lyrics())
        prefix = QStringLiteral("By lyrics: ");
    else if (by_tags())
        prefix = QStringLiteral("By tags: ");

    return QString();
}

QString SearchLimitLayer::token() {
    bool has_predicate = !predicate.isEmpty();
    bool has_genre = !genre.isEmpty();

    if (has_predicate) {
        if (has_genre)
            return tokenPrefix() % predicate % QStringLiteral(" (") % genre % QStringLiteral(") ");
        else
            return tokenPrefix() % predicate;
    }
    else if (has_genre) return genre;
    else if (by_popularity() && req_type == sr_remote) return QStringLiteral("By popularity");
    else return QStringLiteral("All");
}

void SearchLimitLayer::fromJson(const QJsonArray & objs, QList<SearchLimitLayer> & list) {
    for(QJsonArray::ConstIterator request = objs.constBegin(); request != objs.constEnd(); request++) {
        QJsonObject obj = (*request).toObject();

        list << SearchLimitLayer(
            (SearchRequestType)obj.value(JSON_SEARCH_REQUEST_TYPE).toInt(),
             obj.value(JSON_SEARCH_CONTEXT).toVariant(),
            (DataMediaType)obj.value(JSON_SEARCH_CONTENT_TYPE).toInt(),
            (SearchPredicateType)obj.value(JSON_SEARCH_PREDICATE_TYPE).toInt(),
            obj.value(JSON_SEARCH_PREDICATE).toString(),
            obj.value(JSON_SEARCH_GENRE).toString(),
            obj.value(JSON_SEARCH_ITEMS_LIMIT).toInt(),
            obj.value(JSON_SEARCH_OFFSET).toInt(),
            obj.value(JSON_SEARCH_REQUESTS_LIMIT).toInt()
        );
    }
}

void SearchLimitLayer::save(QJsonArray & arr) {
    QJsonObject self;

    self.insert(JSON_SEARCH_REQUEST_TYPE, req_type);
    self.insert(JSON_SEARCH_CONTENT_TYPE, sc_type);
    self.insert(JSON_SEARCH_PREDICATE_TYPE, predicate_type);

    self.insert(JSON_SEARCH_CONTEXT, QJsonValue::fromVariant(context));
    self.insert(JSON_SEARCH_ITEMS_LIMIT, items_limit);
    self.insert(JSON_SEARCH_REQUESTS_LIMIT, requests_limit);

    if (start_offset != 0)
        self.insert(JSON_SEARCH_OFFSET, start_offset);

    if (!predicate.isEmpty())
        self.insert(JSON_SEARCH_PREDICATE, predicate);

    if (!genre.isEmpty())
        self.insert(JSON_SEARCH_GENRE, genre);

    arr << self;
}
