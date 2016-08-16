#ifndef SOUNDCLOUD_SET
#define SOUNDCLOUD_SET

#include "soundcloud_defines.h"

#define SOUNDCLOUD_ALL_GENRES_PARAM QStringLiteral("all-music")

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Set : public Base {
            public:
                enum SetType { set_new_hot = 1, set_top_50, set_popular };

                QString setTypeToStr(const SetType & stype) {
                    switch(stype) {
                        case set_new_hot: return QStringLiteral("New & Hot: ");
                        case set_top_50: return QStringLiteral("Top 50: ");
                        case set_popular: return QStringLiteral("Popular: ");
                        default: return QStringLiteral("Unknown: ");
                    }
                }

                QMap<QString, QString> siteOptions() {
                    return {
                        { QStringLiteral("All Genres"),                 QStringLiteral("all-music") },
                        { QStringLiteral("Alternative Rock"),           QStringLiteral("alternativerock") },
                        { QStringLiteral("Ambient"),                    QStringLiteral("ambient") },
                        { QStringLiteral("Classical"),                  QStringLiteral("classical") },
                        { QStringLiteral("Country"),                    QStringLiteral("country") },
                        { QStringLiteral("Dance & EDM"),                QStringLiteral("danceedm") },
                        { QStringLiteral("Dancehall"),                  QStringLiteral("dancehall") },
                        { QStringLiteral("Deep House"),                 QStringLiteral("deephouse") },
                        { QStringLiteral("Disco"),                      QStringLiteral("disco") },
                        { QStringLiteral("Drum & Bass"),                QStringLiteral("drumbass") },
                        { QStringLiteral("Dubstep"),                    QStringLiteral("dubstep") },
                        { QStringLiteral("Electronic"),                 QStringLiteral("electronic") },
                        { QStringLiteral("Folk & Singer-Songwriter"),   QStringLiteral("folksingersongwriter") },
                        { QStringLiteral("Hip-hop & Rap"),              QStringLiteral("hiphoprap") },
                        { QStringLiteral("House"),                      QStringLiteral("house") },
                        { QStringLiteral("Indie"),                      QStringLiteral("indie") },
                        { QStringLiteral("Jazz & Blues"),               QStringLiteral("jazzblues") },
                        { QStringLiteral("Latin"),                      QStringLiteral("latin") },
                        { QStringLiteral("Metal"),                      QStringLiteral("metal") },
                        { QStringLiteral("Piano"),                      QStringLiteral("piano") },
                        { QStringLiteral("Pop"),                        QStringLiteral("pop") },
                        { QStringLiteral("R&B & Soul"),                 QStringLiteral("rbsoul") },
                        { QStringLiteral("Reggae"),                     QStringLiteral("reggae") },
                        { QStringLiteral("Reggaeton"),                  QStringLiteral("reggaeton") },
                        { QStringLiteral("Rock"),                       QStringLiteral("rock") },
                        { QStringLiteral("Soundtrack"),                 QStringLiteral("soundtrack") },
                        { QStringLiteral("Techno"),                     QStringLiteral("techno") },
                        { QStringLiteral("Trance"),                     QStringLiteral("trance") },
                        { QStringLiteral("Trap"),                       QStringLiteral("trap") },
                        { QStringLiteral("Triphop"),                    QStringLiteral("triphop") },
                        { QStringLiteral("World"),                      QStringLiteral("world") }
                    };
                }

                QJsonValue setByType(const QUrlQuery & attrs) {
                    return setByType(
                        (SetType)attrs.queryItemValue(CMD_SET_TYPE).toInt(),
                        SearchLimit::fromICmdParams(attrs)
                    );
                }

                QJsonValue setByType(const SetType & set_type, const SearchLimit & limits) {
                    Permissions perm = permissions(pr_media_content);
                    QueriableResponse response;

                    switch(perm) {
                        case perm_api:
                        case perm_site: {
                            switch(set_type) {
                                case set_popular: {
                                    response = pRequest(
                                        baseUrlStr(
                                            qst_api_def, path_tracks,
                                            trackSearchQuery(QString(), limits.genre == SOUNDCLOUD_ALL_GENRES_PARAM ? QString() : limits.genre, true)
                                        ),
                                        call_type_json, rules(limits.start_offset, limits.items_limit), 0, proc_json_patch
                                    );
                                break;}

                                case set_new_hot:
                                case set_top_50: {
                                    QJsonObject obj = sRequest(
                                        baseUrlStr(
                                            qst_site_alt1, QStringLiteral("charts"),
                                            {
                                                { tkn_limit, SOUNDCLOUD_PER_REQUEST_LIMIT },
                                                { QStringLiteral("genre"), QString(QStringLiteral("soundcloud:genres:") % (limits.genre.isEmpty() ? SOUNDCLOUD_ALL_GENRES_PARAM : limits.genre)) },
                                                { QStringLiteral("kind"), set_type == set_new_hot ? QStringLiteral("trending") : QStringLiteral("top") }
                                            }
                                        ),
                                        call_type_json, 0, proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                                    );
                                    QJsonArray block_content; // "last_updated":"2016-07-01T05:27:38Z"

                                    QJsonArray collection = obj.value(QStringLiteral("collection")).toArray();
                                    for(QJsonArray::Iterator it = collection.begin(); it != collection.end(); it++)
                                        block_content.append((*it).toObject().value(QStringLiteral("track")));

                                    return prepareBlock(dmt_set, block_content);
                                break;}
                            }
                        }

                        default: Logger::obj().write("Soundcloud", "SET BY TYPE is not accessable", true);
                    }

                    return prepareBlock(dmt_set, cmd_mtd_set_by_type, response, limits, {{CMD_SET_TYPE, set_type}});
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_SET
