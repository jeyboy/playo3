#ifndef TREE_VIEW_H
#define TREE_VIEW_H

#include "../view_interface.h"
#include "tree_model.h"

namespace Playo3 {
    class TreeView : public IView {
      Q_OBJECT
    public:
        TreeView(QWidget * parent, ViewSettings settins, QJsonObject * hash = 0);
        TreeView(IModel * newModel, QWidget * parent, ViewSettings settings);
        ~TreeView();
    };
}

#endif // TREE_VIEW_H
