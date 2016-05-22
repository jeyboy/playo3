#ifndef VK_MODEL_H
#define VK_MODEL_H

#include "modules/models/web/web_model.h"
#include "modules/core/misc/format.h"

namespace Models {
    class VkModel : public WebModel {
        Q_OBJECT
    public:
        inline VkModel(const Params & settings, QJsonObject * hash = 0, QObject * parent = 0) : WebModel(settings, hash, parent) {}
        inline ISearchable * api() const { return &Vk::Api::obj(); }
    public slots:
        void refresh(bool retryPlaing = false);
//        virtual void refreshWall();

    protected slots:
        void proceedWallList(QJsonArray &);

        void proceedAudioList(QJsonObject &);
        void proceedAudioListAndRetry(QJsonObject &);
    };
}

#endif // VK_MODEL_H
