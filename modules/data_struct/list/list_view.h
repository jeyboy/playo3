#ifndef LIST_VIEW_H
#define LIST_VIEW_H

#include "model/view.h"
#include "list_model.h"

class ListView : public View {
  Q_OBJECT
public:
    ListView(QWidget * parent, CBHash settinsSet, QJsonObject * attrs = 0);
    ~ListView();
};

#endif // LIST_VIEW_H
