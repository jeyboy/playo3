#ifndef SOUNDCLOUD_MODEL_H
#define SOUNDCLOUD_MODEL_H

#include "modules/models/web/web_model.h"

namespace Models {
    class SoundcloudModel : public WebModel {
        Q_OBJECT

        void proceedFriendsList(const QJsonArray & friends);
    public:       
        inline SoundcloudModel(const QString & uid, QJsonObject * hash = 0, QObject * parent = 0) : WebModel(uid, hash, parent) {}

        inline Data::Type playlistType() const { return Data::soundcloud; }
        inline ISearchable * api() { return &Soundcloud::Api::obj(); }
    public slots:
        void refresh(bool retryPlaing = false);

    protected slots:
        void proceedAudioList(QJsonObject &);
        void proceedAudioListAndRetry(QJsonObject &);
//        int proceedAudioList(QJsonArray &, FolderItem *);
    };
}

#endif // SOUNDCLOUD_MODEL_H
