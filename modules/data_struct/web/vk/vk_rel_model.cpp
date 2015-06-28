#include "vk_rel_model.h"

using namespace Playo3;
/////////////////////////////////////////////////////////////

VkRelModel::VkRelModel(RelType rtype, QString uid, QJsonObject * hash, QObject * parent)
    : VkModel(uid, hash, parent), rel_type(rtype) {}

VkRelModel::~VkRelModel() {}

void VkRelModel::refresh(bool /*retryPlaing*/) {
    if (QDateTime::currentMSecsSinceEpoch() - lastRefresh < UPDATE_INTERVAL) return;

    lastRefresh = QDateTime::currentMSecsSinceEpoch();
    emit moveInProcess();
    QApplication::processEvents();

    VkApi::instance() -> audioRecomendations(
        tab_uid,
        rel_type == user_rel,
        true,
        Func(this, "proceedAudioList")
    );
}
