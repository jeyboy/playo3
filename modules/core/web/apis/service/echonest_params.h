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

                void pack(QUrlQuery & query, QString & attr_name, QList<ParamWithPower> & objs) {
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

                void toParams(QUrlQuery & query) {
                    artist.initParams(query);
                    if (!artistLocation.isEmpty()) query.addQueryItem(QStringLiteral("artist_location"), artistLocation);
                    if (gsm) gsm -> initParams(query);
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
