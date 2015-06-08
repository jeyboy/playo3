#ifndef ECHONEST_ARTIST_API
#define ECHONEST_ARTIST_API

#include "iapi.h"

//POSSIBLE BUCKETS

//biographies 	returns up to the 15 most recent biographies found on the web related to the artist
//blogs 	returns up to the 15 most recent blogs found on the web related to the artist
//discovery 	returns the discovery score for the artist. This is a measure of how unexpectedly popular the artist is.
//discovery_rank 	returns the discovery rank for the artist
//doc_counts 	returns document counts for each of the various artist document types
//familiarity 	returns the familiarity for the artist
//familiarity_rank 	returns the familiarity rank for the artist
//genre 	returns all genres for an artist
//hotttnesss 	returns the hotttnesss for the artist
//hotttnesss_rank 	returns the hotttnesss rank for the artist
//images 	returns up to the 15 most recent images found on the web related to the artist
//artist_location 	returns information about the location of origin for the artist
//news 	returns up to the 15 most recent news articles found on the web related to the artist
//reviews 	returns up to the 15 most recent reviews found on the web related to the artist
//songs 	returns up to the 15 hotttest songs for the artist
//urls 	returns links to this artist's pages on various sites
//video 	returns up to the 15 most recent videos found on the web related to the artist
//years_active 	returns years active information for the artist
//id:rosetta-catalog 	returns catalog specific information about the artist for the given catalog. See Project Rosetta Stone for details.
//id:Taste-Profile-ID 	returns personal catalog specific information about the artist for the given catalog. See Project Rosetta Stone for details.


class EchonestArtistApi : public IApi {
    public:
        inline QUrl artistBiographiesUrl(QString name, QString id = QString(), int limit = requestLimit()) {
            QUrl url(baseUrl("artist/biographies"));
            QUrlQuery query = buildDefaultParams();
            setLimit(query, limit, 0);
            if (!name.isEmpty())
                setParam(query, "name", name);
            else
                setParam(query, "id", id);

            url.setQuery(query);
            return url;
        }

        QJsonArray artistBiographie(QString artist, QString id = QString(), int limit = 1) {
            QJsonObject response;

            if (proceedQuery(artistBiographiesUrl(artist, id, limit), response))
                return response.value("response").toObject().value("biographies").toArray();

            return QJsonArray();
        }

        //{
        //    "response": {
        //        "status": {
        //            "version": "4.2",
        //            "code": 0,
        //            "message": "Success"
        //        },
        //        "start": 0,
        //        "total": 12,
        //        "biographies": [{
        //            "text": "\"Radiohead\" are an English alternative rock band from Abingdon, Oxfordshire, formed in 1985. The band consists of Thom Yorke (vocals, guitars, piano), Jonny Greenwood (guitars, keyboards, other instruments), Ed O'Brien (guitars, backing vocals), Colin Greenwood (bass, synthesisers) and Phil Selway (drums, percussion).",
        //            "site": "wikipedia",
        //            "url": "http://en.wikipedia.org/wiki/Radiohead",
        //            "license": {
        //                "type": "cc-by-sa",
        //                "attribution": "n/a",
        //                "url": ""
        //            }
        //        }]
        //    }
        //}




        inline QUrl artistFamiliarityUrl(QString name, QString id = QString()) {
            QUrl url(baseUrl("artist/familiarity"));
            QUrlQuery query = buildDefaultParams();
            if (!name.isEmpty())
                setParam(query, "name", name);
            else
                setParam(query, "id", id);

            url.setQuery(query);
            return url;
        }

        QJsonObject artistFamiliarity(QString artist, QString id = QString()) {
            QJsonObject response;

            if (proceedQuery(artistFamiliarityUrl(artist, id), response))
                return response.value("response").toObject().value("artist").toObject();

            return QJsonObject();
        }

        //{
        //  "response": {
        //    "status": {
        //      "code": "0",
        //      "message": "Success",
        //      "version": "4.2"
        //    },
        //    "artist": {
        //      "familiarity": 0.9007877498316712,
        //      "id": "ARH6W4X1187B99274F",
        //      "name": "Radiohead"
        //    }
        //  }
        //}


        //types - overall, social, reviews, mainstream
        inline QUrl artistHotttnesssUrl(QString name, QString id = QString(), QString type = "overall") {
            QUrl url(baseUrl("artist/hotttnesss"));
            QUrlQuery query = buildDefaultParams();
            if (!name.isEmpty())
                setParam(query, "name", name);
            else
                setParam(query, "id", id);

            setParam(query, "type", type);

            url.setQuery(query);
            return url;
        }

        QJsonObject artistHotttnesss(QString artist, QString id = QString()) {
            QJsonObject response;

            if (proceedQuery(artistHotttnesssUrl(artist, id), response))
                return response.value("response").toObject().value("artist").toObject();

            return QJsonObject();
        }

        //{
        //  "response": {
        //    "status": {
        //      "code": "0",
        //      "message": "Success",
        //      "version": "4.2"
        //    },
        //    "artist": {
        //      "hotttnesss": 0.61081915371492912,
        //      "name": "Radiohead",
        //      "id": "ARH6W4X1187B99274F"
        //    }
        //  }
        //}



        inline QUrl artistImagesUrl(QString name, QString id = QString(), int limit = requestLimit(), int offset = 0) {
            QUrl url(baseUrl("artist/images"));
            QUrlQuery query = buildDefaultParams();
            setLimit(query, limit, offset);

            if (!name.isEmpty())
                setParam(query, "name", name);
            else
                setParam(query, "id", id);

            url.setQuery(query);
            return url;
        }

        QJsonArray artistImages(QString name, QString id = QString(), int limit = 1) {
            QJsonObject response;
            QJsonArray images;
            int offset = 0;

            while (proceedQuery(artistImagesUrl(name, id, limit, offset), response)) {
                images.append(response.value("response").toObject().value("images"));

                offset += limit;//artistsByGenreLimit();

                if (offset >= limit || offset >= extractAmount(response))
                    break;
            }

            return images;
        }

        //{
        //  "response": {
        //    "status": {
        //      "code": "0",
        //      "message": "Success",
        //      "version": "4.2"
        //    },
        //      "start": 0,
        //      "total": 121,
        //      "images": [
        //        {
        //          "url": "http://userserve-ak.last.fm/serve/_/174456.jpg",
        //          "license": {
        //            "url": "",
        //            "attribution": "",
        //            "type": "unknown"
        //          }
        //        }
        //      ]
        //    }
        //}


        inline QUrl artistNewsUrl(QString name, QString id = QString(), int limit = requestLimit(), int offset = 0) {
            QUrl url(baseUrl("artist/news"));
            QUrlQuery query = buildDefaultParams();
            setLimit(query, limit, offset);

            if (!name.isEmpty())
                setParam(query, "name", name);
            else
                setParam(query, "id", id);

            setParam(query, "high_relevance", "true"); // if true only news articles that are highly relevant for this artist will be returned

            url.setQuery(query);
            return url;
        }

        QJsonArray artistNews(QString name, QString id = QString(), int limit = 1) {
            QJsonObject response;
            QJsonArray news;
            int offset = 0;

            while (proceedQuery(artistNewsUrl(name, id, limit, offset), response)) {
                news.append(response.value("response").toObject().value("news"));

                offset += limit;

                if (offset >= limit || offset >= extractAmount(response))
                    break;
            }

            return news;
        }

        //{
        //  "response": {
        //    "status": {
        //      "code": "0",
        //      "message": "Success",
        //      "version": "4.2"
        //    },
        //      "start": 0,
        //      "total": 121,
        //      "news": [
        //        {
        //          "name": "First Listen: The National High Violet",
        //          "url": "http://www.drownedinsound.com/news/4139479-first-listen--the-national-high-violet",
        //          "date_posted": "2010-04-12T00:00:00",
        //          "date_found": "2010-04-12T06:15:29",
        //          "summary": " off a new-found sophistication (think of how <span>Radiohead</span>'s rhythm section developed over their first five records). Menacing cellos saw a triplet at a time, rising up like tall buildings around you, then rest. The contrast between Berninger's minimal, one-note vocals and the rising volume of the layers and layers of instruments builds tension until finally the singer shifts key, recognizing the urgency... and then the music dissolves into an atonal coda.  'Afraid of Everyone' Opening ... ",
        //          "id": "484a6108e649f0251c182381e401f0c6"
        //        }
        //      ]
        //    }
        //}





//        // buckets: biographies, blogs, discovery, discovery_rank, doc_counts, familiarity, familiarity_rank, genre, hotttnesss,
//        // hotttnesss_rank, images, artist_location, news, reviews, songs, terms, urls, video, years_active, id:Rosetta-space
//        inline QUrl artistProfileUrl(QString name, QString id = QString(), QString buckets) {
//            QUrl url(baseUrl("artist/profile"));
//            QUrlQuery query = buildDefaultParams();

//            if (!name.isEmpty())
//                setParam(query, "name", name);
//            else
//                setParam(query, "id", id);

//            setParam(query, "bucket", buckets);

//            url.setQuery(query);
//            return url;
//        }

//        QJsonArray artistProfile(QString name, QString id = QString()) {
//            QJsonObject response;

//            if (proceedQuery(artistGenresUrl(name, id), response)) {
//                return response.value("response").toObject().value("terms").toArray();
//            }

//            return QJsonArray();
//        }

        //{
        //    "response": {
        //        "status": {
        //            "version": "4.2",
        //            "code": 0,
        //            "message": "Success"
        //        },
        //        "artist": {
        //            "foreign_ids": [{
        //                "catalog": "7digital-US",
        //                "foreign_id": "7digital-US:artist:304"
        //            }],
        //            "id": "ARH6W4X1187B99274F",
        //            "name": "Radiohead"
        //        }
        //    }
        //}

        //{
        //    "response": {
        //        "status": {
        //            "version": "4.2",
        //            "code": 0,
        //            "message": "Success"
        //        },
        //        "artist": {
        //            "id": "ARH6W4X1187B99274F",
        //            "name": "Radiohead"
        //            "doc_counts": {
        //                "biographies" : 18,
        //                "blogs" : 256,
        //                "images" : 118,
        //                "news" : 32,
        //                "reviews" : 16,
        //                "songs" : 198,
        //                "videos" : 456
        //            }
        //        }
        //    }
        //}











        inline QUrl artistGenresUrl(QString name, QString id = QString(), QString type = "style") {
            QUrl url(baseUrl("artist/list_terms"));
            QUrlQuery query = buildDefaultParams();

            if (!name.isEmpty())
                setParam(query, "name", name);
            else
                setParam(query, "id", id);

            setParam(query, "type", type);

            url.setQuery(query);
            return url;
        }

        QJsonArray artistGenresForSearch() {
            QJsonObject response;

            if (proceedQuery(artistGenresUrl(name, id), response)) {
                return response.value("response").toObject().value("terms").toArray();
            }

            return QJsonArray();
        }

        //{
        //  "response": {
        //    "status": {
        //      "code": "0",
        //      "message": "Success",
        //      "version": "4.2"
        //    },
        //      "type": "style",
        //      "terms": [
        //        { "name": "rock" },
        //        { "name": "alternative" },
        //        { "name": "jazz" },
        //        { "name": "classical" },
        //        { "name": "indie" }
        //      ]
        //    }
        //}


        QJsonArray artistMoodsForSearch() {
            QJsonObject response;

            if (proceedQuery(artistGenresUrl(name, id, "mood"), response)) {
                return response.value("response").toObject().value("terms").toArray();
            }

            return QJsonArray();
        }

        //{
        //  "response": {
        //    "status": {
        //      "code": "0",
        //      "message": "Success",
        //      "version": "4.2"
        //    },
        //      "type": "mood",
        //      "terms": [
        //        { "name": "happy" },
        //        { "name": "angry" },
        //        { "name": "sad" },
        //        { "name": "relaxing" },
        //        { "name": "excited"  }
        //      ]
        //    }
        //}


//        bucket biographies, blogs, discovery, discovery_rank, doc_counts, familiarity, familiarity_rank, genre, hotttnesss, hotttnesss_rank, images, artist_location, news, reviews, songs, terms, urls, video, years_active, id:Rosetta-space 	indicates what data should be returned with each artist
//        limit true, false 	if 'true', limit the results to any of the given idspaces or catalogs
//        artist_location boston, boston+ma, boston+ma+us, city:washington, region:washington, country:united+states 	the name of the location of interest. Location names can optionally be qualified with a type specifier of 'city', 'region' or 'country'.
//        moods	happy, sad^.5 	a mood like happy or sad. See the method list_terms for details on what moods are currently available
//        description Some examples are: alt-rock,-emo, harp^2. 	a description of the artist. See below for more details on the .
//        genre jazz, metal 	a musical genre such as rock, jazz, or dance pop. See the method list_genres for details on what genres are currently available

//        style 	no 	yes 	jazz, metal^2 	a musical style or genre like rock, jazz, or funky. See the method list_terms for details on what styles are currently available

//        rank_type 	no 	no 	relevance, familiarity 	For search by description, style or mood indicates whether results should be ranked by query relevance or by artist familiarity
//        max_familiarity 	no 	no 	0.0 < familiarity < 1.0 	the maximum familiarity for returned artists
//        min_familiarity 	no 	no 	0.0 < familiarity < 1.0 	the minimum familiarity for returned artists
//        max_hotttnesss 	no 	no 	0.0 < hotttnesss < 1.0 	the maximum hotttnesss for returned artists
//        min_hotttnesss 	no 	no 	0.0 < hotttnesss < 1.0 	the minimum hotttnesss for returned artists
//        artist_start_year_before 	no 	no 	1970, 2011, present 	Matches artists that have an earliest start year before the given value
//        artist_start_year_after 	no 	no 	1970, 2011, present 	Matches artists that have an earliest start year after the given value
//        artist_end_year_before 	no 	no 	1970, 2011, present 	Matches artists that have a latest end year before the given value
//        artist_end_year_after 	no 	no 	1970, 2011, present 	Matches artists that have a latest end year after the given value
//        sort 	no 	no 	familiarity-asc, hotttnesss-asc, familiarity-desc, hotttnesss-desc, artist_start_year-asc, artist_start_year-desc, artist_end_year-asc, artist_end_year-desc, 	specified the sort order of the results


        inline QUrl artistSearchUrl(QString name = QString(), bool fuzzySearch = false, QStringList tags = QStringList(),
                                    QStringList moods = QStringList(), QString artistLocation = QString(),
                                    QStringList genres = QStringList(), QStringList styles = QStringList(),
                                    int limit = requestLimit(), int offset = 0) {
            QUrl url(baseUrl("artist/search"));
            QUrlQuery query = buildDefaultParams();
            setLimit(query, limit, offset);

            if (!artistLocation.isEmpty()) setParam(query, "artist_location", artistLocation);
            if (fuzzySearch) setParam(query, "fuzzy_match", "true");
            if (!name.isEmpty()) setParam(query, "name", name);
            if (!tags.isEmpty()) setParam(query, "description", tags.join(','));
            if (!moods.isEmpty()) setParam(query, "mood", moods.join(','));
            if (!genres.isEmpty()) setParam(query, "genre", genres.join(','));
            if (!styles.isEmpty()) setParam(query, "style", styles.join(','));

            url.setQuery(query);
            return url;
        }

        QJsonArray artistSearch(QString name, QString id = QString(), int limit = 1) {
            QJsonObject response;
            QJsonArray news;
            int offset = 0;

            while (proceedQuery(artistNewsUrl(name, id, limit, offset), response)) {
                news.append(response.value("response").toObject().value("news"));

                offset += limit;

                if (offset >= limit || offset >= extractAmount(response))
                    break;
            }

            return news;
        }
};

#endif // ECHONEST_ARTIST_API
