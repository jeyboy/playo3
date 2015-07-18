#ifndef VK_MODEL_H
#define VK_MODEL_H

#include "modules/data_struct/web/web_model.h"
#include "media/duration.h"

namespace Playo3 {
    class VkModel : public WebModel {
        Q_OBJECT
    public:
        inline VkModel(QString uid, QJsonObject * hash = 0, QObject * parent = 0) : WebModel(uid, hash, parent) {}

        inline ContainerType containerType() const { return vk; }
        inline WebApi * api() { return Vk::Api::instance(); }
    public slots:
        virtual void refresh(bool retryPlaing = false);
//        virtual void refreshWall();

    protected slots:
        void proceedWallList(QJsonArray &);

        void proceedAudioList(QJsonObject &);
        void proceedAudioListAndRetry(QJsonObject &);
    };
}

#endif // VK_MODEL_H
