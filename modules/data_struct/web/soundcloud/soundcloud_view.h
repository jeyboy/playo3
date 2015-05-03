#ifndef SOUNDCLOUD_VIEW_H
#define SOUNDCLOUD_VIEW_H

#include "modules/data_struct/view_interface.h"
#include "soundcloud_model.h"

class SoundcloudView : public IView {
  Q_OBJECT
public:
    SoundcloudView(QWidget * parent, ViewSettings settins, QJsonObject * hash = 0);
    SoundcloudView(WebModel * newModel, QWidget * parent, ViewSettings settings);
    ~SoundcloudView();
};

#endif // SOUNDCLOUD_VIEW_H
