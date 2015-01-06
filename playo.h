#ifndef PLAYO_H
#define PLAYO_H

#include <qdebug.h>
#include "modules/controls/window.h"

namespace Ui {
    class Playo;
}

class Playo : public Playo3::MainWindow {
    Q_OBJECT
public:
    explicit Playo(QWidget *parent = 0);
    ~Playo();

protected:
    void init();
    void initMenuWidget();
private:
    Ui::Playo *ui;
};

#endif // PLAYO_H
