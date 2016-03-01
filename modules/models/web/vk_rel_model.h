#ifndef VK_REL_MODEL_H
#define VK_REL_MODEL_H

#include "vk_model.h"

namespace Models {
    class VkRelModel : public VkModel {
        Q_OBJECT
    public:       
        inline VkRelModel(PlaylistRelType rtype, const QString & uid, QJsonObject * hash = 0, QObject * parent = 0)
            : VkModel(uid, hash, parent), rel_type(rtype) {}

        inline PlaylistType playlistType() const { return playlist_vk_rel; }
    public slots:
        void refresh(bool retryPlaing = false);
        inline void refreshWall() { /*stub*/ }
    private:
        PlaylistRelType rel_type;
    };
}

#endif // VK_REL_MODEL_H
