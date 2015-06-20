#include "web_item.h"

using namespace Playo3;

WebItem::WebItem(QVariant uid, QString filePath, QString fileName, FolderItem * parent, int pos) : IItem(parent, fileName, pos) {
    setUid(uid);
    setPath(filePath);
}
