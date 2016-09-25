#ifndef MODEL_SETTINGS
#define MODEL_SETTINGS

#include "modules/core/data_sub_types.h"
#include <qjsonobject.h>

namespace Models {
    enum ParamFlags : int {
        mpf_none = 0,

        mpf_tab_configurable = 1,
        mpf_source_configurable = 2,
        mpf_stream_configurable = 4,
        mpf_feeds_configurable = 8,
        mpf_common = 16,
        //32
        //64
        mpf_del_file = 128,
        mpf_interactive = 256,
        mpf_auto_play_next = 512,

        mpf_common_playlist = mpf_common | mpf_auto_play_next
    };

    struct Params {
        Params(const Core::DataSubType & dataType = Core::dt_none, const ParamFlags & flags = mpf_auto_play_next, const QString & uniq_id = QString(),
            Core::RecType rec = Core::rec_none) : flags(flags), uid(uniq_id), rec_type(rec), data_type(dataType) { }

        Params(const QJsonObject & obj) {
            flags = (ParamFlags)obj[QStringLiteral("flags")].toInt();
            data_type = (Core::DataSubType)obj[QStringLiteral("type")].toInt();
            uid = obj[QStringLiteral("uid")].toString();
            rec_type = (Core::RecType)obj[QStringLiteral("rec_type")].toInt();
        }

        QJsonObject toJson() {
            QJsonObject obj;

            obj[QStringLiteral("flags")] = flags;
            obj[QStringLiteral("type")] = data_type;
            if (!uid.isEmpty())
                obj[QStringLiteral("uid")] = uid;

            obj[QStringLiteral("rec_type")] = rec_type;

            return obj;
        }

        void setFlag(int flag, bool on = true) {
            if (on)
                flags = (ParamFlags)(flags | flag);
            else
                flags = (ParamFlags)(flags & (~(flag)));
        }
        void setFlags(int new_flags) { flags = (ParamFlags)((flags & 31) | new_flags); }

        bool isAutoPlayNext() { return flags & mpf_auto_play_next; }
        bool isInteractive() { return flags & mpf_interactive; }
        bool isDeleteFile() { return flags & mpf_del_file; }
        bool isCommon() { return flags & mpf_common; }

        bool isConfigurable() { return flags & (mpf_common - 1); }
        bool isTabConfigurable() { return flags & mpf_tab_configurable; }
        bool isSourceConfigurable() { return flags & mpf_source_configurable; }
        bool isStreamConfigurable() { return flags & mpf_stream_configurable; }
        bool isFeedsConfigurable() { return flags & mpf_feeds_configurable; }

        ParamFlags flags;
        QString uid;
        Core::RecType rec_type;
        Core::DataSubType data_type;
    };
}

#endif // VIEW_SETTINGS
