#ifndef LEVEL_TREE_VIEW_H
#define LEVEL_TREE_VIEW_H

#include "../view_interface.h"
#include "level_tree_model.h"

namespace Playo3 {
    class LevelTreeView : public IView {
      Q_OBJECT
    public:
        inline LevelTreeView(QWidget * parent, const ViewSettings & settings, const Json::Obj & hash = Json::Obj())
            : IView(dynamic_cast<IModel *>(new LevelTreeModel(hash)), parent, settings) {}
    };
}
#endif // LEVEL_TREE_VIEW_H
