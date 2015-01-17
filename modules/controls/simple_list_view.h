#ifndef SIMPLE_LIST_VIEW_H
#define SIMPLE_LIST_VIEW_H

#include <QListView>
#include <QKeyEvent>

class SimpleListView : public QListView {
    Q_OBJECT
public:
    explicit SimpleListView(QWidget *parent = 0);
    void removeSelected();

protected:
    void keyPressEvent(QKeyEvent *);
};

#endif // SIMPLE_LIST_VIEW_H
