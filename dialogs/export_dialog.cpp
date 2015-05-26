#include "export_dialog.h"
#include "ui_export_dialog.h"

ExportDialog::ExportDialog(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog) {
    ui -> setupUi(this);
}

ExportDialog::~ExportDialog() {
    delete ui;
}

QStringList ExportDialog::getUids() {
    return ui -> uids -> toPlainText().split(/*QRegularExpression(",")*/ ",", QString::SkipEmptyParts);
}
