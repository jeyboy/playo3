#ifndef YOUTUBE_CHANNEL_H
#define YOUTUBE_CHANNEL_H

#include "youtube_defines.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            class Channel : public virtual Base {
            public:
                QJsonValue videoChannelsByUser(const QUrlQuery & args) {
                    return videoChannelsByUser(args.queryItemValue(CMD_ID));
                }
                QJsonValue videoChannelsByUser(const QString & user_name) {
                    return QJsonObject();
                }

                QJsonValue videoChannelsByCategory(const QUrlQuery & args) {
                    return videoChannelsByCategory(
                        args.queryItemValue(CMD_ID),
                        args.queryItemValue(CMD_OFFSET)
                    );
                }
                QJsonValue videoChannelsByCategory(const QString & category_id, const QString & token = QString()) {
                    return QJsonObject();
                }
            };
        }
    }
}

#endif // YOUTUBE_CHANNEL_H
