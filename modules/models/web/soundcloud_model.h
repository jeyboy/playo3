#ifndef SOUNDCLOUD_MODEL_H
#define SOUNDCLOUD_MODEL_H

#include "modules/models/web/web_model.h"

namespace Models {
    class SoundcloudModel : public WebModel {
        Q_OBJECT
    public:       
        inline SoundcloudModel(const Params & settings, QJsonObject * hash = 0, QObject * parent = 0) : WebModel(settings, hash, parent) {}
        inline ISource * api() const { return &Web::Soundcloud::Queries::obj(); }
    public slots:
        void refresh();

    protected slots:
        void proceedJson(QJsonValue &);
    };
}

#endif // SOUNDCLOUD_MODEL_H
