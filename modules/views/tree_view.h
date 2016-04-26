#ifndef TREE_VIEW_H
#define TREE_VIEW_H

#include "view_interface.h"
#include "modules/models/tree_model.h"

namespace Views {
    class TreeView : public IView {
      Q_OBJECT
    public:
        inline TreeView(QWidget * parent, const Params & settings, QJsonObject * hash = 0)
            : IView(new TreeModel(settings, hash), parent) {}
    };
}

#endif // TREE_VIEW_H
