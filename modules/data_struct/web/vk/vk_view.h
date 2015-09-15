#ifndef VK_VIEW_H
#define VK_VIEW_H

#include "modules/data_struct/view_interface.h"
#include "vk_model.h"

class VkView : public IView {
  Q_OBJECT
public:   
    inline VkView(QWidget * parent, const ViewSettings & settings, const Json::Obj & hash = Json::Obj())
        : IView(dynamic_cast<IModel *>(new VkModel(settings.uid, hash)), parent, settings) {}
    inline VkView(WebModel * newModel, QWidget * parent, ViewSettings settings)
        : IView(newModel, parent, settings) {}
};

#endif // VK_VIEW_H
