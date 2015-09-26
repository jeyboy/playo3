#ifndef VK_VIEW_H
#define VK_VIEW_H

#include "modules/views/view_interface.h"
#include "modules/models/web/vk_model.h"

class VkView : public IView {
  Q_OBJECT
public:   
    inline VkView(QWidget * parent, ViewSettings settings, QJsonObject * hash = 0)
        : IView(dynamic_cast<IModel *>(new VkModel(settings.uid, hash)), parent, settings) {}
    inline VkView(WebModel * newModel, QWidget * parent, ViewSettings settings)
        : IView(newModel, parent, settings) {}
};

#endif // VK_VIEW_H
