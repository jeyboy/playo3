#ifndef TREE_VIEW_H
#define TREE_VIEW_H

#include "view_interface.h"
#include "modules/models/tree_model.h"

namespace View {
    class TreeView : public IView {
      Q_OBJECT
    public:
        inline TreeView(QWidget * parent, Params settings, QJsonObject * hash = 0)
            : IView(new TreeModel(hash), parent, settings) {}
//        inline TreeView(IModel * newModel, QWidget * parent, Params settings)
//            : IView(newModel, parent, settings) {}
    };
}

#endif // TREE_VIEW_H
