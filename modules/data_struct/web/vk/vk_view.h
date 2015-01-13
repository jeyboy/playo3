#ifndef VK_VIEW_H
#define VK_VIEW_H

#include "model/view.h"
#include "model/tree/tree_view.h"
#include "model/web/vk/vk_model.h"

class VkView : public TreeView {
  Q_OBJECT
public:
    VkView(QWidget * parent, CBHash settinsSet, QJsonObject * hash = 0);
    ~VkView();

    void removeItem(ModelItem * item);

    QJsonObject toJSON();
};

#endif // VK_VIEW_H
