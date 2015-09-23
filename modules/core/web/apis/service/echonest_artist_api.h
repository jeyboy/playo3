#ifndef ECHONEST_ARTIST_API
#define ECHONEST_ARTIST_API

#include "modules/core/interfaces/iapi.h"

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

namespace Echonest {
    class ArtistApi : virtual public IApi {
        public:
            inline virtual ~ArtistApi() {}

            inline QUrl artistBiographiesUrl(QString & name, QString & id, int limit = DEFAULT_LIMIT_AMOUNT) {
                QUrlQuery query = genDefaultParams();
                setLimit(query, qMin(limit, requestLimit()), 0);
                if (!name.isEmpty())
                    setParam(query, QStringLiteral("name"), name);
                else
                    setParam(query, QStringLiteral("id"), id);

                return baseUrl(QStringLiteral("artist/biographies"), query);
            }

            QJsonArray artistBiographie(QString artist, QString id = QString(), int limit = 1) {
                QJsonObject response;

                if (sQuery(artistBiographiesUrl(artist, id, limit), response))
                    return response.value(QStringLiteral("biographies")).toArray();

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




            inline QUrl artistFamiliarityUrl(QString & name, QString & id) {
                QUrlQuery query = genDefaultParams();
                if (!name.isEmpty())
                    setParam(query, QStringLiteral("name"), name);
                else
                    setParam(query, QStringLiteral("id"), id);

                return baseUrl(QStringLiteral("artist/familiarity"), query);
            }

            QJsonObject artistFamiliarity(QString artist, QString id = QString()) {
                QJsonObject response;

                if (sQuery(artistFamiliarityUrl(artist, id), response))
                    return response.value(QStringLiteral("artist")).toObject();

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
            inline QUrl artistHotttnesssUrl(QString & name, QString & id, QString type = QStringLiteral("overall")) {
                QUrlQuery query = genDefaultParams();
                if (!name.isEmpty())
                    setParam(query, QStringLiteral("name"), name);
                else
                    setParam(query, QStringLiteral("id"), id);

                setParam(query, QStringLiteral("type"), type);

                return baseUrl(QStringLiteral("artist/hotttnesss"), query);
            }

            QJsonObject artistHotttnesss(QString artist, QString id = QString()) {
                QJsonObject response;

                if (sQuery(artistHotttnesssUrl(artist, id), response))
                    return response.value(QStringLiteral("artist")).toObject();

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



            inline QUrl artistImagesUrl(QString & name, QString & id) {
                QUrlQuery query = genDefaultParams();

                if (!name.isEmpty())
                    setParam(query, QStringLiteral("name"), name);
                else
                    setParam(query, QStringLiteral("id"), id);

                return baseUrl(QStringLiteral("artist/images"), query);
            }

            QJsonArray artistImages(QString name, QString id = QString(), int count = 1) {
                return lQuery(
                    artistImagesUrl(name, id),
                    QueryRules(QStringLiteral("images"), requestLimit(), count)
                );
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


            inline QUrl artistNewsUrl(QString & name, QString & id) {
                QUrlQuery query = genDefaultParams();

                if (!name.isEmpty())
                    setParam(query, QStringLiteral("name"), name);
                else
                    setParam(query, QStringLiteral("id"), id);

                setParam(query, QStringLiteral("high_relevance"), QStringLiteral("true")); // if true only news articles that are highly relevant for this artist will be returned

                return baseUrl(QStringLiteral("artist/news"), query);
            }

            QJsonArray artistNews(QString name, QString id = QString(), int count = 1) {
                return lQuery(
                    artistNewsUrl(name, id),
                    QueryRules(QStringLiteral("news"), requestLimit(), count)
                );
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





            // buckets: biographies, blogs, discovery, discovery_rank, doc_counts, familiarity, familiarity_rank, genre, hotttnesss,
            // hotttnesss_rank, images, artist_location, news, reviews, songs, terms, urls, video, years_active, id:Rosetta-space
            inline QUrl artistProfileUrl(QString name, QStringList buckets, QString id = QString()) {
                QUrlQuery query = genDefaultParams();

                if (!name.isEmpty())
                    setParam(query, QStringLiteral("name"), name);
                else
                    setParam(query, QStringLiteral("id"), id);

                setParam(query, QStringLiteral("bucket"), buckets);

                return baseUrl(QStringLiteral("artist/profile"), query);
            }

            QJsonObject artistProfile(QString name, QString id = QString()) {
                QJsonObject response;

                QStringList buckets;
                buckets << QStringLiteral("biographies") << QStringLiteral("familiarity") << QStringLiteral("familiarity_rank")
                        << QStringLiteral("genre") << QStringLiteral("hotttnesss") << QStringLiteral("hotttnesss_rank")
                        << QStringLiteral("images") << QStringLiteral("songs") << QStringLiteral("years_active")
                        << QStringLiteral("news") << QStringLiteral("terms");

                if (sQuery(artistProfileUrl(name, buckets, id), response))
                    return response.value(QStringLiteral("artist")).toObject();

                return QJsonObject();
            }

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



            inline QUrl artistStylesUrl(QString type = QStringLiteral("style")) {
                QUrlQuery query = genDefaultParams();
                setParam(query, QStringLiteral("type"), type);
                return baseUrl(QStringLiteral("artist/list_terms"), query);
            }

            QJsonArray artistStylesForSearch() {
                QJsonObject response;

                if (sQuery(artistStylesUrl(), response))
                    return response.value(QStringLiteral("terms")).toArray();

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

                if (sQuery(artistStylesUrl(QStringLiteral("mood")), response))
                    return response.value(QStringLiteral("terms")).toArray();

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
    //        style jazz, metal^2 	a musical style or genre like rock, jazz, or funky. See the method list_terms for details on what styles are currently available

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


            // did not use name and description at once
            inline QUrl artistSearchUrl(QString & name, bool fuzzySearch, QStringList & tags,
                                        QStringList & moods, QString & artistLocation,
                                        QStringList & genres, QStringList & styles)
            {
                QUrlQuery query = genDefaultParams();

                if (!artistLocation.isEmpty()) setParam(query, QStringLiteral("artist_location"), artistLocation);
                if (fuzzySearch) setParam(query, QStringLiteral("fuzzy_match"), QStringLiteral("true"));
                if (!name.isEmpty()) setParam(query, QStringLiteral("name"), name);
                if (!genres.isEmpty()) setParam(query, QStringLiteral("genre"), genres.join(','));

                setParam(query, QStringLiteral("style"), styles);
                setParam(query, QStringLiteral("description"), tags);
                setParam(query, QStringLiteral("mood"), moods);

                return baseUrl(QStringLiteral("artist/search"), query);
            }

            // need to check
            QJsonArray artistSearch(int count = DEFAULT_LIMIT_AMOUNT, QString name = QString(), bool fuzzySearch = false, QStringList tags = QStringList(),
                                    QStringList moods = QStringList(), QString artistLocation = QString(),
                                    QStringList genres = QStringList(), QStringList styles = QStringList())
            {
                return lQuery(
                    artistSearchUrl(name, fuzzySearch, tags, moods, artistLocation, genres, styles),
                    QueryRules(QStringLiteral("artists"), requestLimit(), count)
                );
            }

            //{
            //  "response": {
            //    "status": {
            //      "code": "0",
            //      "message": "Success",
            //      "version": "4.2"
            //    },
            //    "artists": {
            //      "artist": [
            //        {
            //          "name": "Radiohead",
            //          "id": "ARH6W4X1187B99274F"
            //          "foreign_ids": [
            //                {
            //                    "catalog": "CAXFDYO12E2688C130",
            //                    "foreign_id": "CAXFDYO12E2688C130:artist:item-1"
            //                },
            //                {
            //                    "catalog": "7digital-US",
            //                    "foreign_id": "7digital-US:artist:304"
            //                }
            //            ]
            //          }
            //       ]
            //     }
            //   }
            //}





            inline QUrl artistSongsUrl(QString & name, QString & id) {
                QUrlQuery query = genDefaultParams();

                if (!name.isEmpty())
                    setParam(query, QStringLiteral("name"), name);
                else
                    setParam(query, QStringLiteral("id"), id);

                return baseUrl(QStringLiteral("artist/songs"), query);
            }

            QJsonArray artistSongs(QString name, QString id = QString(), int count = DEFAULT_LIMIT_AMOUNT) {
                return lQuery(
                    artistSongsUrl(name, id),
                    QueryRules(QStringLiteral("songs"), requestLimit(), count)
                );
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
            //      "songs": [
            //        {
            //          "id": "SOXZYYG127F3E1B7A2",
            //          "title": "Karma police"
            //        },
            //        {
            //          "id": "SOXZABD127F3E1B7A2",
            //          "title" : "Creep"
            //        }
            //      ]
            //    }
            //  }
            //}









            //id 	one of id or name 	yes (up to 5) 	ARH6W4X1187B99274F, 7digital-US:artist:304 ARH6W4X1187B99274F^2 	the artist ID. An Echo Nest ID or a Rosetta ID (See Project Rosetta Stone)
            //name 	one of id or name 	yes 	Weezer, the+beatles ,the+beatles^0.5 	the artist name
            //min_results 	no 	no 	0 < results < 100, (Default=15) 	Indicates the minimum number of results to be returned regardless of constraints
            //bucket 	no 	yes 	biographies, blogs, discovery, discovery_rank, doc_counts, familiarity, familiarity_rank, genre, hotttnesss, hotttnesss_rank, images, artist_location, news, reviews, songs, terms, urls, video, years_active, id:Rosetta-space 	indicates what data should be returned with each artist
            //max_familiarity 	no 	no 	0.0 < familiarity < 1.0 	the maximum familiarity for returned artists
            //min_familiarity 	no 	no 	0.0 < familiarity < 1.0 	the minimum familiarity for returned artists
            //max_hotttnesss 	no 	no 	0.0 < hotttnesss < 1.0 	the maximum hotttnesss for returned artists
            //min_hotttnesss 	no 	no 	0.0 < hotttnesss < 1.0 	the minimum hotttnesss for returned artists
            //artist_start_year_before 	no 	no 	1970, 2011, present 	Matches artists that have an earliest start year before the given value
            //artist_start_year_after 	no 	no 	1970, 2011, present 	Matches artists that have an earliest start year after the given value
            //artist_end_year_before 	no 	no 	1970, 2011, present 	Matches artists that have a latest end year before the given value
            //artist_end_year_after 	no 	no 	1970, 2011, present 	Matches artists that have a latest end year after the given value
            //seed_catalog 	no 	yes (up to 5) 	CAKSMUX1321A708AA4 	only give similars to those in a catalog or catalogs, An Echo Nest artist catalog identifier

            inline QUrl artistSimilarsUrl(QStringList & names, QStringList & ids, int min_results) {
                QUrlQuery query = genDefaultParams();

                setParam(query, QStringLiteral("min_results"), QString::number(qMin(min_results, requestLimit())));

                setParam(query, QStringLiteral("name"), names);
                if (!names.isEmpty())
                    setParam(query, QStringLiteral("name"), names);
                else
                    setParam(query, QStringLiteral("id"), ids);

                return baseUrl(QStringLiteral("artist/similar"), query);
            }

            QJsonArray artistSimilars(QStringList names, QStringList ids = QStringList(), int count = DEFAULT_LIMIT_AMOUNT) {
                return lQuery(
                    artistSimilarsUrl(names, ids, count),
                    QueryRules(QStringLiteral("artists"), requestLimit(), count)
                );
            }

            //{
            //  "response": {
            //    "status": {
            //      "code": "0",
            //      "message": "Success",
            //      "version": "4.2"
            //    },
            //      "artists": [
            //        {
            //          "name": "Thom Yorke",
            //          "id": "ARH1N081187B9AC562"
            //        }
            //      ]
            //  }
            //}


            inline QUrl artistSuggestUrl(QString & name_part, int limit = 15) {
                QUrlQuery query = genDefaultParams();
                setLimit(query, qMin(limit, 15), 0);
                setParam(query, QStringLiteral("name"), name_part);

                return baseUrl(QStringLiteral("artist/suggest"), query);
            }

            QJsonArray artistSuggests(QString name_part, int limit = 15) {
                QJsonObject response;

                if (sQuery(artistSuggestUrl(name_part, limit), response))
                    return response.value(QStringLiteral("artists")).toObject().value(QStringLiteral("artist")).toArray();

                return QJsonArray();
            }

            //{
            //  "response": {
            //    "status": {
            //      "code": "0",
            //      "message": "Success",
            //      "version": "4.2"
            //    },
            //    "artists": {

            //      "artist": [
            //        {
            //            "id": "ARH6W4X1187B99274F",
            //            "name": "Radiohead"
            //        },
            //        {
            //            "id": "ARQCFYC12A10043E5B",
            //            "name": "Go Radio"
            //        },
            //        {
            //            "id": "AREKO1L1187B997EFE",
            //            "name": "The Radio Dept."
            //        },
            //        {
            //            "id": "AR0PK561187B9B9EF9",
            //            "name": "TV on the Radio"
            //        },
            //        {
            //            "id": "ARPCRYQ1187FB4ECB8",
            //            "name": "Joshua Radin"
            //        }
            //      ]
            //    }
            //  }
            //}


    //        sort 	no 	no 	weight, frequency
            inline QUrl artistTermsUrl(QString & name, QString & id, QString sort = QStringLiteral("frequency")) {
                QUrlQuery query = genDefaultParams();

                if (!name.isEmpty())
                    setParam(query, QStringLiteral("name"), name);
                else
                    setParam(query, QStringLiteral("id"), id);

                setParam(query, QStringLiteral("sort"), sort);

                return baseUrl(QStringLiteral("artist/terms"), query);
            }

            QJsonArray artistTerms(QString name, QString id = QString()) {
                QJsonObject response;

                if (sQuery(artistTermsUrl(name, id), response))
                    return response.value(QStringLiteral("terms")).toArray();

                return QJsonArray();
            }

            //{
            //  "response": {
            //    "status": {
            //      "code": "0",
            //      "message": "Success",
            //      "version": "4.2"
            //    },
            //      "terms": [
            //        {
            //          "name": "alternative",
            //          "frequency": 1
            //          "weight": .78
            //        },
            //        {
            //          "name": "rock",
            //          "frequency": .98
            //          "weight": .79
            //        }
            //      ]
            //    }
            //  }
            //}


            //bucket biographies, blogs, discovery, discovery_rank, doc_counts, familiarity, familiarity_rank, genre, hotttnesss, hotttnesss_rank, images, artist_location, news, reviews, songs, terms, urls, video, years_active, id:Rosetta-space
            inline QUrl artistTopUrl(QStringList & names) {
                QUrlQuery query = genDefaultParams();
                setParam(query, QStringLiteral("name"), names);
                return baseUrl(QStringLiteral("artist/top_hottt"), query);
            }

            QJsonArray artistTop(QStringList names, int count = DEFAULT_LIMIT_AMOUNT) {
                return lQuery(
                    artistTopUrl(names),
                    QueryRules(QStringLiteral("artists"), requestLimit(), count)
                );
            }

            //{
            //  "response": {
            //    "status": {
            //      "code": "0",
            //      "message": "Success",
            //      "version": "4.2"
            //    },
            //      "artists": [
            //        {
            //          "id": "ARI3Y821187FB3649C",
            //          "hotttnesss": 0.9733418,
            //          "name": "MGMT"
            //        }
            //      ]
            //  }
            //}




            inline QUrl artistTopTermsUrl(int limit = DEFAULT_LIMIT_AMOUNT) {
                QUrlQuery query = genDefaultParams();
                setLimit(query, qMin(limit, requestLimit()), 0);
                return baseUrl(QStringLiteral("artist/top_terms"), query);
            }

            QJsonArray artistTopTerms(int limit = 999) {
                QJsonObject response;

                if (sQuery(artistTopTermsUrl(limit), response))
                    return response.value(QStringLiteral("terms")).toArray();

                return QJsonArray();
            }
    };
}

#endif // ECHONEST_ARTIST_API
