#include "playo.h"
#include "ui_playo.h"

Playo::Playo(QWidget *parent) :
    QMainWindow(parent), ui(new Ui::Playo) {
    ui->setupUi(this);

    QApplication::setWindowIcon(QIcon(":icon"));
    setWindowTitle("Playo");
    setAcceptDrops(true);
    setAttribute(Qt::WA_DeleteOnClose);

    init();
}

Playo::~Playo() {
    delete ui;
}

void init() {

}
