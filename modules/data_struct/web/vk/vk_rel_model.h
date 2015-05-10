#ifndef VK_REL_MODEL_H
#define VK_REL_MODEL_H

#include "vk_model.h"
#include "vk_rel_types.h"

namespace Playo3 {
    class VkRelModel : public VkModel {
        Q_OBJECT
    public:
        VkRelModel(RelType rtype, QString uid, QJsonObject * hash = 0, QObject * parent = 0);
        ~VkRelModel();

        inline ContainerType containerType() const { return vk_rel; }
    public slots:
        void refresh(bool retryPlaing = false);
        inline void refreshWall() { /*stub*/ }
    private:
        RelType rel_type;
    };
}

#endif // VK_REL_MODEL_H
