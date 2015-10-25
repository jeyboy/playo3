#ifndef OD_VIEW_H
#define OD_VIEW_H

#include "modules/views/view_interface.h"
#include "modules/models/web/od_model.h"

namespace Views {
    class OdView : public IView {
      Q_OBJECT
    public:
        inline OdView(QWidget * parent, Params settings, QJsonObject * hash = 0)
            : IView(new OdModel(settings.uid, hash), parent, settings) {}
//        inline OdView(WebModel * newModel, QWidget * parent, Params settings)
//            : IView(newModel, parent, settings) {}
    };
}

#endif // SOUNDCLOUD_VIEW_H
