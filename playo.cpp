#include "playo.h"
#include "ui_playo.h"
#include <QLabel>
#include <QGridLayout>
#include <QPushButton>

Playo::Playo(QWidget * parent) : Playo3::MainWindow(parent), ui(new Ui::Playo) {
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
    addToolBar("lalka");
    addToolBar("lalka2");
}

void Playo::initMenuWidget() {
    QLabel *coolLabel = new QLabel("The coolest text in the world", this);
    QWidget *menuWidget = new QWidget;
    QGridLayout *vLayout = new QGridLayout();
    menuWidget -> setLayout(vLayout);
    menuWidget -> setMouseTracking(true);
    vLayout -> addWidget(coolLabel, 0, 0, Qt::AlignLeft);
    QPushButton * close = new QPushButton("Close", this);
    connect(close, SIGNAL(clicked()), this, SLOT(close()));
    vLayout -> addWidget(close, 0, 1, Qt::AlignRight);
    setMenuWidget(menuWidget);
}
