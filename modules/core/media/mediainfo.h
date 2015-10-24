#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include <qurl.h>

#include "taglib.h"
#include "tag.h"
#include "fileref.h"

#include "modules/core/misc/file_utils/extensions.h"
#include "modules/core/media/interfaces/imediainfo.h"

#include "settings.h"

namespace Core {
    namespace Media {
        class MediaInfo : public IMediaInfo {
        public:
            MediaInfo(const QUrl & uri, const QVariant & extension, bool onlyTags = true);
            inline ~MediaInfo() { delete fileName; }
            void initInfo();
        private:
            void readInfo(TagLib::FileRef f);

            TagLib::FileName * fileName;
        };
    }
}

#endif // MEDIAINFO_H
