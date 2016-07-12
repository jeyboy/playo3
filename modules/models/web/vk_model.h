#ifndef VK_MODEL_H
#define VK_MODEL_H

#include "modules/models/web/web_model.h"
#include "modules/core/misc/format.h"

namespace Models {
    class VkModel : public WebModel {
        Q_OBJECT
    public:
        inline VkModel(const Params & settings, QJsonObject * hash = 0, QObject * parent = 0) : WebModel(settings, hash, parent) {}
        inline ISource * api() const { return &Vk::Queries::obj(); }
    public slots:
        void refresh();
    protected slots:
        void proceedJson(QJsonValue &);
    };
}

#endif // VK_MODEL_H
