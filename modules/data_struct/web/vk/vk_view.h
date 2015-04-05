#ifndef VK_VIEW_H
#define VK_VIEW_H

#include "modules/data_struct/view_interface.h"
#include "vk_model.h"

class VkView : public IView {
  Q_OBJECT
public:
    VkView(QWidget * parent, ViewSettings settins, QJsonObject * hash = 0);
    VkView(WebModel * newModel, QWidget * parent, ViewSettings settings);
    ~VkView();
};

#endif // VK_VIEW_H
