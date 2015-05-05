#ifndef VK_MODEL_H
#define VK_MODEL_H

#include "modules/web/socials_api/vk_api.h"
#include "modules/data_struct/web/web_model.h"
#include "media/duration.h"

namespace Playo3 {
    class VkModel : public WebModel {
        Q_OBJECT

    public:
        VkModel(QString uid, QJsonObject * hash = 0, QObject * parent = 0);
        ~VkModel();

        inline ContainerType containerType() const { return vk; }
        inline WebApi * api() { return VkApi::instance(); }
    public slots:
        void refresh(bool retryPlaing = false);
        void refreshWall();

    protected slots:
        void proceedWallList(QJsonObject &);

        void proceedAudioList(QJsonObject &);
        void proceedAudioListAndRetry(QJsonObject &);
        int proceedAudioList(QJsonArray &, FolderItem *);
    //    void proceedAudioListUpdate(QJsonObject &, QHash<QString, IItem *> &);
    };
}

#endif // VK_MODEL_H
