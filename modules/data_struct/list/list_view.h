#ifndef LIST_VIEW_H
#define LIST_VIEW_H

#include "../view_interface.h"
#include "list_model.h"

namespace Playo3 {
    class ListView : public IView {
      Q_OBJECT
    public:
        ListView(QWidget * parent, ViewSettings settins, QJsonObject * attrs = 0);
        ~ListView();
    };
}

#endif // LIST_VIEW_H
