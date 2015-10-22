#include "vk_rel_model.h"

using namespace Models;
/////////////////////////////////////////////////////////////
void VkRelModel::refresh(bool /*retryPlaing*/) {
//    if (QDateTime::currentMSecsSinceEpoch() - lastRefresh < UPDATE_INTERVAL) return;

//    lastRefresh = QDateTime::currentMSecsSinceEpoch();
    emit moveInProcess();
    QApplication::processEvents();

    Vk::Api::obj().audioRecomendations(
        tab_uid,
        rel_type == Data::user_rel,
        true,
        new Func(this, SLOT(proceedAudioList(QJsonObject &)))
    );
}
