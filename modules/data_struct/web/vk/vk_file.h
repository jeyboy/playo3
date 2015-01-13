#ifndef VK_FILE_H
#define VK_FILE_H

#include <QFile>
#include <QJsonObject>

#include "model/model_item.h"

class VkFile : public ModelItem {
public:
    VkFile(QJsonObject *hash, ModelItem *parent = 0);
    VkFile(const QString filepath, QString filename, QString ownerID, QString fileID, ModelItem *parent = 0, int genre_id = -1, QString itemDuration = "", int itemSize = -1, QString itemInfo = "", int init_state = STATE_DEFAULT | STATE_CHECKED);
    ~VkFile();

    bool removePhysicalObject();

    bool isExist() const;
    bool isRemote() const;

    QString toUID();
    QUrl toUrl();
    QJsonObject toJSON();
protected:
    QString uid;
    QString owner_uid;
};


#endif // VK_FILE_H
