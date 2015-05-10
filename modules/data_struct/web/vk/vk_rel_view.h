#ifndef VK_REL_VIEW_H
#define VK_REL_VIEW_H

#include "vk_rel_model.h"
#include "vk_view.h"

class VkRelView : public VkView {
  Q_OBJECT
public:
    VkRelView(QWidget * parent, ViewSettings settins, QJsonObject * hash = 0);
    ~VkRelView();
};

#endif // VK_REL_VIEW_H
