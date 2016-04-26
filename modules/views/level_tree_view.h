#ifndef LEVEL_TREE_VIEW_H
#define LEVEL_TREE_VIEW_H

#include "view_interface.h"
#include "modules/models/level_tree_model.h"

namespace Views {
    class LevelTreeView : public IView {
      Q_OBJECT
    public:
        inline LevelTreeView(QWidget * parent, const Params & settings, QJsonObject * hash = 0)
            : IView(new LevelTreeModel(settings, hash), parent) {}
    };
}
#endif // LEVEL_TREE_VIEW_H
