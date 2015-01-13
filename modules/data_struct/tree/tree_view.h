#ifndef TREE_VIEW_H
#define TREE_VIEW_H

#include "model/view.h"
#include "model/tree/tree_model.h"

class TreeView : public View {
  Q_OBJECT
public:
    TreeView(QWidget * parent, CBHash settinsSet, QJsonObject * hash = 0);
    TreeView(Model * newModel, QWidget *parent, CBHash settingsSet);
    ~TreeView();
};

#endif // TREE_VIEW_H
