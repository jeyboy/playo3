#ifndef OD_VIEW_H
#define OD_VIEW_H

#include "modules/views/view_interface.h"
#include "modules/models/web/od_model.h"

interface View {
    class OdView : public IView {
      Q_OBJECT
    public:
        inline OdView(QWidget * parent, ViewSettings settings, QJsonObject * hash = 0)
            : IView(dynamic_cast<IModel *>(new OdModel(settings.uid, hash)), parent, settings) {}
        inline OdView(WebModel * newModel, QWidget * parent, ViewSettings settings)
            : IView(newModel, parent, settings) {}
    };
}

#endif // SOUNDCLOUD_VIEW_H
