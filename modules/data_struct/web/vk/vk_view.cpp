#include "vk_view.h"
#include <QDebug>

VkView::VkView(QWidget *parent, CBHash settingsSet, QJsonObject *hash)
    : TreeView(dynamic_cast<Model *>(new VkModel(QString::number(settingsSet.value("uid")), hash)), parent, settingsSet) {
}

VkView::~VkView() {}

void VkView::removeItem(ModelItem * item) {
    QString uid = item -> toUID();
    if (!uid.isEmpty())
        ((VkModel *)model) -> addRemovedUID(uid);

    View::removeItem(item);
}

QJsonObject VkView::toJSON() {
    QJsonObject res = TreeView::toJSON();

    VkModel * vk_model = ((VkModel *)model);

    res.insert("uid", vk_model -> getTabUid());
    res.insert("deleted", vk_model -> deletedToJson());

    return res;
}
