#include "toolbardialog.h"
#include "ui_toolbardialog.h"

ToolbarDialog::ToolbarDialog(QString title, QWidget * parent) :
    QDialog(parent), ui(new Ui::ToolbarDialog) {
    ui -> setupUi(this);

    setWindowTitle(title);
}

ToolbarDialog::~ToolbarDialog() {
    delete ui;
}

QString ToolbarDialog::getName() {
    return ui -> toolbarName -> text();
}

void ToolbarDialog::setName(QString name) {
    ui -> toolbarName -> setText(name);
}
