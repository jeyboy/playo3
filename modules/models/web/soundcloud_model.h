#ifndef SOUNDCLOUD_MODEL_H
#define SOUNDCLOUD_MODEL_H

#include "modules/models/web/web_model.h"

namespace Models {
    class SoundcloudModel : public WebModel {
        Q_OBJECT

        void proceedFriendsList(const QJsonArray & friends);
    public:       
        inline SoundcloudModel(const Params & settings, QJsonObject * hash = 0, QObject * parent = 0) : WebModel(settings, hash, parent) {}
        inline ISearchable * api() const { return &Soundcloud::Api::obj(); }
    public slots:
        void refresh(bool retryPlaing = false);

    protected slots:
        void proceedAudioList(QJsonObject &);
    };
}

#endif // SOUNDCLOUD_MODEL_H
