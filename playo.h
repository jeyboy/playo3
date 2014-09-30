#ifndef PLAYO_H
#define PLAYO_H

#include <QMainWindow>

namespace Ui {
    class Playo;
}

class Playo : public QMainWindow {
    Q_OBJECT
public:
    explicit Playo(QWidget *parent = 0);
    ~Playo();

protected:
    void init();
private:
    Ui::Playo *ui;
};

#endif // PLAYO_H
