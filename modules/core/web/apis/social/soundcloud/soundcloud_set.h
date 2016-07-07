#ifndef SOUNDCLOUD_ISET
#define SOUNDCLOUD_ISET

#include "soundcloud_defines.h"

namespace Core {
    namespace Web {
        namespace Soundcloud {
            class Set : public Base {
            protected:
                enum SetType { new_hot = 1, top_50, popular };

                QJsonValue setByType(const SetType & setType, const QString & genre = QString()) {
                    Permissions perm = permissions(pr_media_content);

                    switch(perm) {
                        case perm_api:
                        case perm_site: {
                            switch(setType) {
                                case popular: {
                                    return pRequest(
                                        baseUrlStr(
                                            qst_api_def, path_tracks,
                                            trackSearchQuery(QString(), genre, true)
                                        ),
                                        call_type_json, rules(), 0, proc_json_patch
                                    );
                                }

                                case new_hot:
                                case top_50: {
                                    QJsonObject obj = sRequest(
                                        baseUrlStr(
                                            qst_site_alt1, QStringLiteral("charts"),
                                            {
                                                { QStringLiteral("genre"), QString(QStringLiteral("soundcloud:genres:") % (genre.isEmpty() ? QStringLiteral("all-music") : genre)) },
                                                { QStringLiteral("kind"), setType == new_hot ? QStringLiteral("trending") : QStringLiteral("top") }
                                            }
                                        ),
                                        call_type_json, 0, proc_json_patch, IQUERY_DEF_FIELDS, call_method_get, headers()
                                    );
                                    QJsonArray res; // "last_updated":"2016-07-01T05:27:38Z"

                                    QJsonArray collection = obj.value(QStringLiteral("collection")).toArray();
                                    for(QJsonArray::Iterator it = collection.begin(); it != collection.end(); it++)
                                        res.append((*it).toObject().value(QStringLiteral("track")));

                                    return res;
                                }
                            }
                        }

                        default: Logger::obj().write("Soundcloud", "SET BY TYPE is not accessable", true);
                    }

                    return QJsonArray();
                }
            };
        }
    }
}

#endif // SOUNDCLOUD_ISET
