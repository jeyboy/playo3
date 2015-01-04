#include "toolbarbuttondialog.h"
#include "ui_toolbarbuttondialog.h"

ToolbarButtonDialog::ToolbarButtonDialog(QWidget *parent) :
    QDialog(parent), ui(new Ui::ToolbarButtonDialog) {
    ui->setupUi(this);

    setWindowTitle("Toolbar button settings");
}

ToolbarButtonDialog::~ToolbarButtonDialog() {
    delete ui;
}

QString ToolbarButtonDialog::getName() {
    return ui -> buttonName -> text();
}

void ToolbarButtonDialog::setName(QString name) {
    ui -> buttonName -> setText(name);
}

QString ToolbarButtonDialog::getPath() {
    return ui -> buttonPath -> text();
}
void ToolbarButtonDialog::setPath(QString name) {
    ui -> buttonPath -> setText(name);
}

void ToolbarButtonDialog::on_browseButton_clicked() {
    QString path = QFileDialog::getExistingDirectory(this, tr("Choose path"));
    setPath(path);
}
