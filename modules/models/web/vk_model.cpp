#include "vk_model.h"

using namespace Models;
/////////////////////////////////////////////////////////////
void VkModel::refresh() {
    emit moveInProcess();
    QApplication::processEvents();

    if (sttngs.rec_type == Core::rec_none) {
        Vk::Queries::obj().userInfoAsync(
            sttngs.uid,
            new Func(this, SLOT(proceedJson(QJsonValue &)))
        );
    } else {
        Vk::Queries::obj().trackRecommendationsAsync(
            sttngs.uid,
            sttngs.rec_type == Core::rec_user,
            true,
            new Func(this, SLOT(proceedJson(QJsonValue &)))
        );
    }
}

void VkModel::proceedJson(QJsonValue & hash) {
    proceedBlocks(hash.toArray(), rootItem);
    emit moveOutProcess();
}
