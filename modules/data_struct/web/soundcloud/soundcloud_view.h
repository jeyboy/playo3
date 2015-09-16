#ifndef SOUNDCLOUD_VIEW_H
#define SOUNDCLOUD_VIEW_H

#include "modules/data_struct/view_interface.h"
#include "soundcloud_model.h"

class SoundcloudView : public IView {
  Q_OBJECT
public:
    inline SoundcloudView(QWidget * parent, const ViewSettings & settings, Json::Obj * hash = 0)
        : IView(dynamic_cast<IModel *>(new SoundcloudModel(settings.uid, hash)), parent, settings) {}
    inline SoundcloudView(WebModel * newModel, QWidget * parent, ViewSettings settings)
        : IView(newModel, parent, settings) {}
};

#endif // SOUNDCLOUD_VIEW_H
