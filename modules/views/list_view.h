#ifndef LIST_VIEW_H
#define LIST_VIEW_H

#include "view_interface.h"
#include "modules/models/list_model.h"

namespace Views {
    class ListView : public IView {
      Q_OBJECT
    public:
        inline ListView(QWidget * parent, Params settings, QJsonObject * hash = 0)
            : IView(new ListModel(hash), parent, settings) {
            setIndentation(4); // -2
        }
    };
}

#endif // LIST_VIEW_H
