#ifndef WEB_VIEW_H
#define WEB_VIEW_H

#include "modules/views/view_interface.h"
#include "modules/models/web/web_model.h"

namespace Views {
    class WebView : public IView {
      Q_OBJECT
    public:
        inline WebView(QWidget * parent, const Params & settings, QJsonObject * hash = 0)
            : IView(new WebModel(settings, hash), parent) {}
    };
}

#endif // WEB_VIEW_H
