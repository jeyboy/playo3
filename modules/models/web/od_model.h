#ifndef OD_MODEL_H
#define OD_MODEL_H

#include "modules/data_struct/web/web_model.h"
#include "media/duration.h"

namespace Playo3 {
    class OdModel : public WebModel {
        Q_OBJECT
    public:       
        inline OdModel(QString uid, QJsonObject * hash = 0, QObject * parent = 0) : WebModel(uid, hash, parent) {}

        inline ContainerType containerType() const { return od; }
        inline WebApi * api() { return Od::Api::instance(); }
    public slots:
        void refresh(bool retryPlaing = false);

    protected slots:
        void proceedAudioList(QJsonObject &);
        void proceedAudioListAndRetry(QJsonObject &);
//        int proceedAudioList(QJsonArray &, FolderItem *);
    };
}

#endif // OD_MODEL_H
