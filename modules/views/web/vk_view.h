#ifndef VK_VIEW_H
#define VK_VIEW_H

#include "modules/views/view_interface.h"
#include "modules/models/web/vk_model.h"

namespace Views {
    class VkView : public IView {
      Q_OBJECT
    public:
        inline VkView(QWidget * parent, const Params & settings, QJsonObject * hash = 0)
            : IView(new VkModel(settings, hash), parent) {}
    };
}

#endif // VK_VIEW_H
