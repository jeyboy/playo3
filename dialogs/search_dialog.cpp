#include "search_dialog.h"
#include "ui_search_dialog.h"

#include <qmessagebox.h>

SearchDialog::SearchDialog(QWidget * parent) : BaseDialog(parent), ui(new Ui::SearchDialog) {
    ui -> setupUi(this);

    QGridLayout * newLayout = new QGridLayout(this);
    QLabel * err_output = new QLabel(this);

    configurator = new Controls::SearchConfigurator(this, ui -> acceptButton, err_output);

    newLayout -> addWidget(err_output, 0, 0, 1, 2);
    newLayout -> addWidget(configurator, 1, 0, 1, 2);
    newLayout -> addWidget(ui -> cancelButton, 2, 0);
    newLayout -> addWidget(ui -> acceptButton, 2, 1);
    setLayout(newLayout);

    setMinimumSize(480, 480);
    setMaximumSize(480, 480);
}

SearchDialog::~SearchDialog() {
    delete ui;
}

Core::SearchLimitLayers SearchDialog::params() {
    return configurator -> params();
}

void SearchDialog::on_cancelButton_clicked() { reject(); }

void SearchDialog::on_acceptButton_clicked() {
    QString err = configurator -> validate();

    if (err.isEmpty())
        accept();
    else
        QMessageBox::warning(this, QLatin1String("Warning"), err);
}
