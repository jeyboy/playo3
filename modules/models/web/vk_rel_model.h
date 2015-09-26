#ifndef VK_REL_MODEL_H
#define VK_REL_MODEL_H

#include "vk_model.h"

namespace Model {
    class VkRelModel : public VkModel {
        Q_OBJECT
    public:       
        inline VkRelModel(Data::RelType rtype, QString uid, QJsonObject * hash = 0, QObject * parent = 0)
            : VkModel(uid, hash, parent), rel_type(rtype) {}

        inline Data::Type playlistType() const { return Data::vk_rel; }
    public slots:
        void refresh(bool retryPlaing = false);
        inline void refreshWall() { /*stub*/ }
    private:
        Data::RelType rel_type;
    };
}

#endif // VK_REL_MODEL_H
