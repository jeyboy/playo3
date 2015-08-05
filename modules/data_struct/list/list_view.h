#ifndef LIST_VIEW_H
#define LIST_VIEW_H

#include "../view_interface.h"
#include "list_model.h"

namespace Playo3 {
    class ListView : public IView {
      Q_OBJECT
    public:
        inline ListView(QWidget * parent, ViewSettings settings, QJsonObject * hash = 0)
            : IView(dynamic_cast<IModel *>(new ListModel(hash)), parent, settings) {
            setIndentation(4);
        }
    };
}

#endif // LIST_VIEW_H
