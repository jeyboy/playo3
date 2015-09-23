#ifndef SOUNDCLOUD_MODEL_H
#define SOUNDCLOUD_MODEL_H

#include "modules/data_struct/web/web_model.h"
#include "media/duration.h"

namespace Playo3 {
    class SoundcloudModel : public WebModel {
        Q_OBJECT
    public:       
        inline SoundcloudModel(QString uid, QJsonObject * hash = 0, QObject * parent = 0) : WebModel(uid, hash, parent) {}

        inline ContainerType containerType() const { return soundcloud; }
        inline WebApi * api() { return Soundcloud::Api::instance(); }
    public slots:
        void refresh(bool retryPlaing = false);

    protected slots:
        void proceedAudioList(QJsonObject &);
        void proceedAudioListAndRetry(QJsonObject &);
//        int proceedAudioList(QJsonArray &, FolderItem *);
    };
}

#endif // SOUNDCLOUD_MODEL_H
