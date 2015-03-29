#ifndef VK_MODEL_H
#define VK_MODEL_H

//#include "web/socials/vk_api.h"
#include "modules/data_struct/web/web_model.h"
//#include "media/media_player/utils/duration.h"

class VkModel : public WebModel {
    Q_OBJECT

public:
    VkModel(QString uid, QJsonObject * hash = 0, QObject * parent = 0);
    ~VkModel();

    inline ContainerType containerType() const { return vk; }
//    inline WebApi * getApi() { return VkApi::instance(); }
public slots:
//    void refresh();
//    void refreshWall();

protected slots:
//    void proceedWallList(QJsonObject &);

//    void proceedAudioList(QJsonObject &);
//    void proceedAudioList(QJsonArray &, ModelItem *, QHash<ModelItem*, QString> &);
//    void proceedAudioListUpdate(QJsonObject &, QHash<ModelItem *, QString> &);
protected:
    QModelIndex dropProcession(const QList<QUrl> & list);

    void filesRoutine(ModelItem * index, QFileInfo currFile);
    void filesRoutine(ModelItem * index, QList<QUrl> list);
};


#endif // VK_MODEL_H
