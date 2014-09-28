#include "playo.h"
#include "ui_playo.h"

Playo::Playo(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Playo) {
    ui->setupUi(this);


}

Playo::~Playo() {
    delete ui;
}
