#ifndef FOURSHARED_VIEW_H
#define FOURSHARED_VIEW_H

#include "modules/views/view_interface.h"
#include "modules/models/web/fourshared_model.h"

namespace Views {
    using namespace Models;

    class FoursharedView : public IView {
      Q_OBJECT
    public:
        inline FoursharedView(QWidget * parent, const Params & settings, QJsonObject * hash = 0)
            : IView(new FoursharedModel(settings, hash), parent) {}
    };
}

#endif // FOURSHARED_VIEW_H
