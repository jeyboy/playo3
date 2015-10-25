#ifndef SOUNDCLOUD_VIEW_H
#define SOUNDCLOUD_VIEW_H

#include "modules/views/view_interface.h"
#include "modules/models/web/soundcloud_model.h"

namespace Views {
    using namespace Models;

    class SoundcloudView : public IView {
      Q_OBJECT
    public:
        inline SoundcloudView(QWidget * parent, Params settings, QJsonObject * hash = 0)
            : IView(new SoundcloudModel(settings.uid, hash), parent, settings) {}
//        inline SoundcloudView(WebModel * newModel, QWidget * parent, Params settings)
//            : IView(newModel, parent, settings) {}
    };
}

#endif // SOUNDCLOUD_VIEW_H
