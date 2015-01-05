#include "playo.h"
#include "ui_playo.h"
#include <QDockWidget>

#include "modules/controls/button.h"

using namespace Playo3;

Playo::Playo(QWidget * parent) : MainWindow(parent), ui(new Ui::Playo) {
    ui -> setupUi(this);

    QApplication::setWindowIcon(QIcon(":ico"));
    setWindowTitle("Playo");
    setAcceptDrops(true);

    //    setAttribute(Qt::WA_DeleteOnClose);

    init();
}

Playo::~Playo() {
    delete ui;
}

void Playo::init() {
    addDockWidget(Qt::TopDockWidgetArea, new QDockWidget("pido", this));
    addToolBar("lalka");
    addToolBar("lalka2");
}
