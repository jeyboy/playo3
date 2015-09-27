#include "vk_rel_model.h"

using namespace Models;
/////////////////////////////////////////////////////////////
void VkRelModel::refresh(bool /*retryPlaing*/) {
    if (QDateTime::currentMSecsSinceEpoch() - lastRefresh < UPDATE_INTERVAL) return;

    lastRefresh = QDateTime::currentMSecsSinceEpoch();
    emit moveInProcess();
    QApplication::processEvents();

    Vk::Api::instance() -> audioRecomendations(
        tab_uid,
        rel_type == user_rel,
        true,
        Func(this, "proceedAudioList")
    );
}
