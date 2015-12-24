#ifndef VK_ITEM_H
#define VK_ITEM_H

#include "web_file.h"

namespace Core {
    class VkFile : public WebFile {
    public:        
        inline VkFile(QJsonObject * hash, Playlist * parent = 0) : WebFile(hash, parent) {}
        inline VkFile(QVariantMap & hash, Playlist * parent = 0, int pos = -1) : WebFile(hash, parent, pos) {}
        inline VkFile(const QVariant & uid, const QString & filePath, const QString & fileName, Playlist * parent = 0, int pos = -1)
            : WebFile(uid, filePath, fileName, parent, pos) {
            setExtension(QStringLiteral("mp3"));
        }

        inline int itemType() const { return VK_FILE; }
        inline bool isShareable() const { return true; }
    };
}

#endif // VK_ITEM_H
