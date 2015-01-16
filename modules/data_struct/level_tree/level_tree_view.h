#ifndef LEVEL_TREE_VIEW_H
#define LEVEL_TREE_VIEW_H

#include "../view_interface.h"
#include "level_tree_model.h"

namespace Playo3 {
    class LevelTreeView : public ViewInterface {
      Q_OBJECT
    public:
        LevelTreeView(QWidget * parent, ViewSettings settins, QJsonObject * attrs = 0);
        ~LevelTreeView();
    };
}
#endif // LEVEL_TREE_VIEW_H
