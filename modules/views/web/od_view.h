#ifndef OD_VIEW_H
#define OD_VIEW_H

#include "modules/views/view_interface.h"
#include "modules/models/web/od_model.h"

namespace Views {
    class OdView : public IView {
      Q_OBJECT
    public:
        inline OdView(QWidget * parent, const Params & settings, QJsonObject * hash = 0)
            : IView(new OdModel(settings, hash), parent) {}
    };
}

#endif // SOUNDCLOUD_VIEW_H
