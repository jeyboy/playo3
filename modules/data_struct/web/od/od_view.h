#ifndef OD_VIEW_H
#define OD_VIEW_H

#include "modules/data_struct/view_interface.h"
#include "od_model.h"

class OdView : public IView {
  Q_OBJECT
public:
    inline OdView(QWidget * parent, const ViewSettings & settings, Json::Obj * hash = 0)
        : IView(dynamic_cast<IModel *>(new OdModel(settings.uid, hash)), parent, settings) {}
    inline OdView(WebModel * newModel, QWidget * parent, const ViewSettings & settings)
        : IView(newModel, parent, settings) {}
};

#endif // SOUNDCLOUD_VIEW_H
