#include "vk_rel_view.h"

using namespace Playo3;

VkRelView::VkRelView(QWidget * parent, ViewSettings settings, QJsonObject * hash)
    : VkView(dynamic_cast<VkModel *>(new VkRelModel(settings.rel_type, settings.uid, hash)), parent, settings) {
}

VkRelView::~VkRelView() {}
