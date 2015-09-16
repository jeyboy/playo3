#ifndef VK_REL_VIEW_H
#define VK_REL_VIEW_H

#include "vk_rel_model.h"
#include "vk_view.h"

class VkRelView : public VkView {
  Q_OBJECT
public:   
    inline VkRelView(QWidget * parent, const ViewSettings & settings, Json::Obj * hash = 0)
        : VkView(dynamic_cast<VkModel *>(new VkRelModel(settings.rel_type, settings.uid, hash)), parent, settings) {}
};

#endif // VK_REL_VIEW_H
