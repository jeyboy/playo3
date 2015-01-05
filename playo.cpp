#include "playo.h"
#include "ui_playo.h"
#include <QLabel>
#include <QDockWidget>
#include <QMenuBar>

#include "modules/controls/button.h"

using namespace Playo3;

Playo::Playo(QWidget * parent) : MainWindow(parent), ui(new Ui::Playo) {
    ui -> setupUi(this);

    QApplication::setWindowIcon(QIcon(":ico"));
    setWindowTitle("Playo");
    setAcceptDrops(true);

    //    setAttribute(Qt::WA_DeleteOnClose);

    initMenuWidget();
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

void Playo::initMenuWidget() {
    QLabel *coolLabel = new QLabel("The coolest text in the world", menuWidget);
    ((QGridLayout *)menuWidget -> layout()) -> addWidget(coolLabel, 0, 0, Qt::AlignLeft);
    ((QGridLayout *)menuWidget -> layout()) -> addWidget(new Button("Close", this, SLOT(close()), menuWidget), 0, 1, Qt::AlignRight | Qt::AlignVCenter);
//    setMenuWidget(menuWidget);
}
