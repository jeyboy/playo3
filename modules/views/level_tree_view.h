#ifndef LEVEL_TREE_VIEW_H
#define LEVEL_TREE_VIEW_H

#include "view_interface.h"
#include "modules/models/level_tree_model.h"

namespace Playo3 {
    class LevelTreeView : public IView {
      Q_OBJECT
    public:
        inline LevelTreeView(QWidget * parent, ViewSettings settings, QJsonObject * hash = 0)
            : IView(dynamic_cast<IModel *>(new LevelTreeModel(hash)), parent, settings) {}
    };
}
#endif // LEVEL_TREE_VIEW_H
