#ifndef ECHONEST_PARAMS
#define ECHONEST_PARAMS

#include <qstring.h>
#include <qurlquery.h>

namespace Core {
    namespace Web {
        namespace Echonest {
            struct IntervalParam {
                float min; // 0
                float max; // 0.999

                void initParams(QUrlQuery & query, const QString & postfix) {
                    if (min > 0) query.addQueryItem(QStringLiteral("min_") % postfix, QString::number(min));
                    if (max < 1) query.addQueryItem(QStringLiteral("max_") % postfix, QString::number(max));
                }
            };

            struct ParamWithPower {
                QString name;
                float power;

                QString toStr() {
                    if (power == 0) return name;
                    if (power > 0) return name % '^' % QString::number(power);
                    return '-' % name;
                }
            };

            struct DGSMParams {
                QList<ParamWithPower> descriptions;
                QStringList genres;
                QList<ParamWithPower> styles;
                QList<ParamWithPower> moods;

                void initParams(QUrlQuery & query) {
                    if (!genres.isEmpty()) query.addQueryItem(QStringLiteral("genre"), genres.join(','));

                    pack(query, QStringLiteral("style"), styles);
                    pack(query, QStringLiteral("description"), descriptions);
                    pack(query, QStringLiteral("mood"), moods);
                }

                static void pack(QUrlQuery & query, QString & attr_name, QList<ParamWithPower> & objs) {
                    for(QList<ParamWithPower>::ConstIterator val = objs.constBegin(); val != objs.constEnd(); val++)
                        query.addQueryItem(attr_name, (*val).toStr());
                }
            };

            struct Artist {
                inline Artist(const QString & name) : name(name) {}

                inline bool initParams(QUrlQuery & query) {
                    bool is_present = !name.isEmpty();
                    if (is_present)
                        query.addQueryItem(QStringLiteral("name"), name);

                    return is_present;
                }

                QString name;
            };

            struct ArtistCharacter {
                inline ArtistCharacter(const QString & name, const QString & id = QString()) : artist(Artist(name)), id(id) {}

                inline void initParams(QUrlQuery & query) {
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

                void initParams(QUrlQuery & query) {
                    artist.initParams(query);
                    if (!artistLocation.isEmpty()) query.addQueryItem(QStringLiteral("artist_location"), artistLocation);
                    if (gsm) gsm -> initParams(query);
                    if (familiarity) familiarity -> initParams(query, QStringLiteral("familiarity"));
                    if (hotttnesss) hotttnesss -> initParams(query, QStringLiteral("hotttnesss"));
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

                void initParams(QUrlQuery & query) {
//                  setParam(query, QStringLiteral("id"), ids);
                    DGSMParams::pack(query, QStringLiteral("name"), names);

                    if (familiarity) familiarity -> initParams(query, QStringLiteral("familiarity"));
                    if (hotttnesss) hotttnesss -> initParams(query, QStringLiteral("hotttnesss"));
                }

                ~ArtistParams() {
                    delete gsm;
                    delete familiarity;
                    delete hotttnesss;
                }
            };
        }
    }
}

#endif // ECHONEST_SETTINGS
