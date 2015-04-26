#ifndef SIMPLE_LIST_VIEW_H
#define SIMPLE_LIST_VIEW_H

#include <qlistview.h>
#include <qevent.h>

class SimpleListView : public QListView {
    Q_OBJECT
public:
    explicit SimpleListView(QWidget *parent = 0);
    void removeSelected();

protected:
    void keyPressEvent(QKeyEvent *);
};

#endif // SIMPLE_LIST_VIEW_H
