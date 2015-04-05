#include "vk_view.h"

using namespace Playo3;

VkView::VkView(QWidget * parent, ViewSettings settings, QJsonObject * hash)
    : IView(dynamic_cast<IModel *>(new VkModel(settings.uid, hash)), parent, settings) {}

VkView::VkView(WebModel * newModel, QWidget * parent, ViewSettings settings) : IView(newModel, parent, settings) {}

VkView::~VkView() {}
