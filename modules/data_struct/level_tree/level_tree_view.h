#ifndef LEVEL_TREE_VIEW_H
#define LEVEL_TREE_VIEW_H

#include "model/view.h"
#include "level_tree_model.h"

class LevelTreeView : public View {
  Q_OBJECT
public:
    LevelTreeView(QWidget * parent, CBHash settinsSet, QJsonObject * attrs = 0);
    ~LevelTreeView();
};
#endif // LEVEL_TREE_VIEW_H
