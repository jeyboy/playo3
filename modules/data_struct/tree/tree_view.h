#ifndef TREE_VIEW_H
#define TREE_VIEW_H

#include "../view_interface.h"
#include "tree_model.h"

namespace Playo3 {
    class TreeView : public IView {
      Q_OBJECT
    public:
        inline TreeView(QWidget * parent, const ViewSettings & settings, Json::Obj * hash = 0)
            : IView(dynamic_cast<IModel *>(new TreeModel(hash)), parent, settings) {}
        inline TreeView(IModel * newModel, QWidget * parent, const ViewSettings & settings)
            : IView(newModel, parent, settings) {}
    };
}

#endif // TREE_VIEW_H
