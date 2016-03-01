#include "vk_rel_model.h"

using namespace Models;
/////////////////////////////////////////////////////////////
void VkRelModel::refresh(bool retryPlaing) {
//    if (QDateTime::currentMSecsSinceEpoch() - lastRefresh < UPDATE_INTERVAL) return;

//    lastRefresh = QDateTime::currentMSecsSinceEpoch();

    if (rel_type == playlist_none_rel) {
        VkModel::refresh(retryPlaing);
    } else {
        emit moveInProcess();
        QApplication::processEvents();

        Vk::Api::obj().audioRecomendations(
            tab_uid,
            rel_type == playlist_user_rel,
            true,
            new Func(this, SLOT(proceedAudioList(QJsonObject &)))
        );
    }
}
