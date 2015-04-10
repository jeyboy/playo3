#include "vk_view.h"

using namespace Playo3;

VkView::VkView(QWidget * parent, ViewSettings settings, QJsonObject * hash)
    : IView(dynamic_cast<IModel *>(new VkModel(settings.uid, hash)), parent, settings) {
    if (!hash)
        (qobject_cast<VkModel *>(mdl)) -> refresh();
}

VkView::VkView(WebModel * newModel, QWidget * parent, ViewSettings settings) : IView(newModel, parent, settings) {}

VkView::~VkView() {}
