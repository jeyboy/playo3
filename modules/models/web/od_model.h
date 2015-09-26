#ifndef OD_MODEL_H
#define OD_MODEL_H

#include "web_model.h"

namespace Model {
    class OdModel : public WebModel {
        Q_OBJECT
    public:       
        inline OdModel(const QString & uid, QJsonObject * hash = 0, QObject * parent = 0) : WebModel(uid, hash, parent) {}

        inline Data::Type playlistType() const { return Data::od; }
        inline WebApi * api() { return Od::Api::instance(); }
    public slots:
        void refresh(bool retryPlaing = false);

    protected slots:
        void proceedAudioList(QJsonObject &);
        void proceedAudioListAndRetry(QJsonObject &);
//        int proceedAudioList(QJsonArray &, Playlist *);
    };
}

#endif // OD_MODEL_H