#ifndef FOURSHARED_MODEL_H
#define FOURSHARED_MODEL_H

#include "modules/models/web/web_model.h"

namespace Models {
    class FoursharedModel : public WebModel {
        Q_OBJECT
    public:       
        inline FoursharedModel(const Params & settings, QJsonObject * hash = 0, QObject * parent = 0) : WebModel(settings, hash, parent) {}
        inline ISearchable * api() const { return &Web::Fourshared::Api::obj(); }
    public slots:
        void refresh();
    protected slots:
        void proceedJson(QJsonObject &);
    };
}

#endif // FOURSHARED_MODEL_H
