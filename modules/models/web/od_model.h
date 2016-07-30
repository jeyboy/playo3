#ifndef OD_MODEL_H
#define OD_MODEL_H

#include "web_model.h"

namespace Models {
    class OdModel : public WebModel {
        Q_OBJECT
    public:       
        inline OdModel(const Params & settings, QJsonObject * hash = 0, QObject * parent = 0) : WebModel(settings, hash, parent) {}
        inline ISource * api() const { return &Od::Queries::obj(); }
    public slots:
        void refresh();

    protected slots:
        void proceedJson(QJsonValue &);
    };
}

#endif // OD_MODEL_H
