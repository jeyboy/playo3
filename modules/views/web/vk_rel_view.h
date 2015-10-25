#ifndef VK_REL_VIEW_H
#define VK_REL_VIEW_H

#include "modules/models/web/vk_rel_model.h"
#include "vk_view.h"

namespace Views {
    class VkRelView : public VkView {
      Q_OBJECT
    public:
        inline VkRelView(QWidget * parent, Params settings, QJsonObject * hash = 0)
            : VkView(new VkRelModel(settings.rel_type, settings.uid, hash), parent, settings) {}
    };
}

#endif // VK_REL_VIEW_H
