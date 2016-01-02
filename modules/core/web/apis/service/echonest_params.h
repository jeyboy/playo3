#ifndef ECHONEST_PARAMS
#define ECHONEST_PARAMS

#include <qstring.h>
#include <qurlquery.h>

namespace Core {
    namespace Web {
        namespace Echonest {
            struct SongTypeParam {
                QString name;
                QString state;

                SongTypeParam(const QString & name, const QString & state) : name(name), state(state) {}

                void initParams(QUrlQuery & query) const {
                    query.addQueryItem(QStringLiteral("song_type"), name % QStringLiteral(":") % state);
                }
            };

            struct SongTypeParamsList : public QList<SongTypeParam> {
                void initParams(QUrlQuery & query) const {
                    qDebug() << "&&&&&&&&&&&&&&&&&&&&& " << size();
                    for(QList<SongTypeParam>::ConstIterator sType = constBegin(); sType != constEnd(); sType++)
                        (*sType).initParams(query);
                }
            };

            struct IntervalParam {
                float min; // 0
                float max; // 0.999
                float top;

                IntervalParam(float min, float max, float top = .999) : min(min), max(max), top(top) {}

                void initParams(QUrlQuery & query, const QString & mask) {
                    if (min > 0) query.addQueryItem(mask.arg("min"), QString::number(min));
                    if (max < top) query.addQueryItem(mask.arg("max"), QString::number(max));
                }
            };

            struct ParamWithPower {
                QString name;
                float power;

                ParamWithPower(const QString & name, float power) : name(name), power(power) {}

                QString toStr() const {
                    if (power == 0) return name;
                    if (power > 0) return name % '^' % QString::number(power);
                    return '-' % name;
                }
            };

            struct DGSMParams {
                QList<ParamWithPower> descriptions;
                QList<ParamWithPower> styles;
                QList<ParamWithPower> moods;
                QStringList genres;

                DGSMParams(const QList<ParamWithPower> & descriptions = QList<ParamWithPower>(),
                    const QList<ParamWithPower> & styles = QList<ParamWithPower>(),
                    const QList<ParamWithPower> & moods = QList<ParamWithPower>(),
                    const QStringList genres = QStringList()) :
                    descriptions(descriptions), styles(styles), moods(moods), genres(genres)
                {}

                void initParams(QUrlQuery & query) {
                    if (!genres.isEmpty()) query.addQueryItem(QStringLiteral("genre"), genres.join(','));

                    DGSMParams::pack(query, QStringLiteral("style"), styles);
                    DGSMParams::pack(query, QStringLiteral("description"), descriptions);
                    DGSMParams::pack(query, QStringLiteral("mood"), moods);
                }

                static void pack(QUrlQuery & query, const QString & attr_name, const QList<ParamWithPower> & objs) {
                    for(QList<ParamWithPower>::ConstIterator val = objs.constBegin(); val != objs.constEnd(); val++)
                        query.addQueryItem(attr_name, (*val).toStr());
                }
            };

            struct Artist {
                inline Artist(const QString & name) : name(name) {}

                inline bool initParams(QUrlQuery & query) const {
                    bool is_present = !name.isEmpty();
                    if (is_present)
                        query.addQueryItem(QStringLiteral("name"), name);

                    return is_present;
                }

                QString name;
            };

            struct ArtistCharacter {
                inline ArtistCharacter(const QString & name, const QString & id = QString()) : artist(Artist(name)), id(id) {}

                inline void initParams(QUrlQuery & query) const {
                    if (!artist.initParams(query))
                        query.addQueryItem(QStringLiteral("id"), id);
                }

                Artist artist;
                QString id;
            };

            struct ArtistParams {
                Artist artist;
                QString artistLocation;
                DGSMParams * gsm;
                IntervalParam * familiarity;
                IntervalParam * hotttnesss;

                inline ArtistParams(const QString & artist = QString(), const QString & artistLocation = QString(),
                    DGSMParams * gsm = 0, IntervalParam * familiarity = 0, IntervalParam * hotttnesss = 0) :
                    artist(artist), artistLocation(artistLocation), gsm(gsm), familiarity(familiarity), hotttnesss(hotttnesss) {}

                void initParams(QUrlQuery & query) const {
                    artist.initParams(query);
                    if (!artistLocation.isEmpty()) query.addQueryItem(QStringLiteral("artist_location"), artistLocation);
                    if (gsm) gsm -> initParams(query);
                    if (familiarity) familiarity -> initParams(query, QStringLiteral("%1_familiarity"));
                    if (hotttnesss) hotttnesss -> initParams(query, QStringLiteral("%1_hotttnesss"));
                }

                ~ArtistParams() {
                    delete gsm;
                    delete familiarity;
                    delete hotttnesss;
                }
            };

            struct ArtistSimilarityParams {
                QList<ParamWithPower> artists;
//                QList<ParamWithPower> ids;

                IntervalParam * familiarity;
                IntervalParam * hotttnesss;

                inline ArtistSimilarityParams(const QList<ParamWithPower> artists, IntervalParam * familiarity = 0,
                    IntervalParam * hotttnesss = 0) : artists(artists), familiarity(familiarity), hotttnesss(hotttnesss) {}

                void initParams(QUrlQuery & query) const {
//                  setParam(query, QStringLiteral("id"), ids);
                    DGSMParams::pack(query, QStringLiteral("name"), artists);

                    if (familiarity) familiarity -> initParams(query, QStringLiteral("%1_familiarity"));
                    if (hotttnesss) hotttnesss -> initParams(query, QStringLiteral("%1_hotttnesss"));
                }

                ~ArtistSimilarityParams() {
                    delete familiarity;
                    delete hotttnesss;
                }
            };

            struct SongSearchParams {
                Artist * artist;
                QString title;
                bool combined;

                int mode;
                SongTypeParamsList songTypes;

                QString artist_start_year_after;
                QString artist_end_year_after;

                DGSMParams * gsm; // genres is not used here

                IntervalParam * artist_familiarity;
                IntervalParam * hotttnesss;
                IntervalParam * tempo;
                IntervalParam * danceability;
                IntervalParam * energy;
                IntervalParam * liveness;
                IntervalParam * speechiness;
                IntervalParam * acousticness;

                inline SongSearchParams(
                    Artist * artist = 0, const QString & title = QString(), bool combined = false,
                    const QString & artist_start_year = QString(), const QString & artist_end_year = QString(),
                    int mode = -1, const SongTypeParamsList & songTypes = SongTypeParamsList(),
                    DGSMParams * gsm = 0, IntervalParam * artist_familiarity = 0,
                    IntervalParam * hotttnesss = 0, IntervalParam * tempo = 0,
                    IntervalParam * danceability = 0,
                    IntervalParam * energy = 0, IntervalParam * liveness = 0,
                    IntervalParam * speechiness = 0, IntervalParam * acousticness = 0) :
                    artist(artist), title(title), combined(combined),
                    mode(mode), songTypes(songTypes), artist_start_year_after(artist_start_year),
                    artist_end_year_after(artist_end_year), gsm(gsm), artist_familiarity(artist_familiarity),
                    hotttnesss(hotttnesss), tempo(tempo), danceability(danceability),
                    energy(energy), liveness(liveness),
                    speechiness(speechiness), acousticness(acousticness)
                {}

                void initParams(QUrlQuery & query) const {
                    if (combined) {
                        if (!artist) return;
                        query.addQueryItem(QStringLiteral("combined"), artist -> name % ' ' % title);
                    } else {
                        if (artist) artist -> initParams(query);
                        if (!title.isEmpty()) query.addQueryItem(QStringLiteral("title"), title);
                    }

                    if (mode == 0 || mode == 1) query.addQueryItem(QStringLiteral("mode"), QString::number(mode));


                    songTypes.initParams(query);

                    if (!artist_start_year_after.isEmpty())
                        query.addQueryItem(QStringLiteral("artist_start_year_after"), artist_start_year_after);

                    if (!artist_end_year_after.isEmpty())
                        query.addQueryItem(QStringLiteral("artist_end_year_after"), artist_end_year_after);

                    if (gsm) gsm -> initParams(query);

                    if (hotttnesss) hotttnesss -> initParams(query, QStringLiteral("%1_hotttnesss"));
                    if (tempo) tempo -> initParams(query, QStringLiteral("%1_tempo"));
                    if (artist_familiarity) artist_familiarity -> initParams(query, QStringLiteral("artist_%1_familiarity"));
                    if (danceability) danceability -> initParams(query, QStringLiteral("%1_danceability"));
                    if (energy) energy -> initParams(query, QStringLiteral("%1_energy"));
                    if (liveness) liveness -> initParams(query, QStringLiteral("%1_liveness"));
                    if (speechiness) speechiness -> initParams(query, QStringLiteral("%1_speechiness"));
                    if (acousticness) acousticness -> initParams(query, QStringLiteral("%1_acousticness"));
                }

                ~SongSearchParams() {
                    delete artist;
                    delete gsm;
                    delete artist_familiarity;
                    delete hotttnesss;
                    delete tempo;
                    delete danceability;
                    delete energy;
                    delete liveness;
                    delete speechiness;
                    delete acousticness;
                }
            };
        }
    }
}

#endif // ECHONEST_SETTINGS
