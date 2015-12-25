#include "search_dialog.h"
#include "ui_search_dialog.h"
#include "dockbars.h"

using namespace Presentation;

SearchDialog::SearchDialog(QWidget * parent) : BaseDialog(parent), ui(new Ui::SearchDialog) {
    ui -> setupUi(this);

    configurator = new Controls::SearchConfigurator(this, ui -> acceptButton);

    QGridLayout * newLayout = new QGridLayout(this);
    newLayout -> addWidget(configurator, 0, 0, 1, 2);
    newLayout -> addWidget(ui -> cancelButton, 1, 0);
    newLayout -> addWidget(ui -> acceptButton, 1, 1);
    setLayout(newLayout);

    setMinimumSize(480, 480);
    setMaximumSize(480, 480);
}

SearchDialog::~SearchDialog() {
    delete ui;
}

SearchSettings SearchDialog::params() {
    return configurator -> params();
}

void SearchDialog::on_cancelButton_clicked() { reject(); }

void SearchDialog::on_acceptButton_clicked() { accept(); }
