#ifndef SOUNDCLOUD_MODEL_H
#define SOUNDCLOUD_MODEL_H

#include "modules/data_struct/web/web_model.h"
#include "media/duration.h"

namespace Playo3 {
    class SoundcloudModel : public WebModel {
        Q_OBJECT
    public:       
        inline SoundcloudModel(const QString & uid, Json::Obj * hash = 0, QObject * parent = 0) : WebModel(uid, hash, parent) {}

        inline ContainerType containerType() const { return soundcloud; }
        inline WebApi * api() { return Soundcloud::Api::instance(); }
    public slots:
        void refresh(bool retryPlaing = false);

    protected slots:
        void proceedAudioList(Json::Obj &);
        void proceedAudioListAndRetry(Json::Obj &);
//        int proceedAudioList(Json::Arr &, FolderItem *);
    };
}

#endif // SOUNDCLOUD_MODEL_H
