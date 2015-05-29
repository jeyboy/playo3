#include "export_dialog.h"
#include "ui_export_dialog.h"

ExportDialog::ExportDialog(QWidget * parent) :
    QDialog(parent),
    ui(new Ui::ExportDialog) {
    ui -> setupUi(this);

    highlighter = new IdHighlighter(ui -> uids -> document());
}

ExportDialog::~ExportDialog() {
    delete ui;
}

QStringList ExportDialog::getUids() {
    return ui -> uids -> toPlainText().split(QRegularExpression("\\W"), QString::SkipEmptyParts);
}

void ExportDialog::on_uids_textChanged() {
//    ui -> uids -> blockSignals(true);
//    ui -> uids -> setPlainText(ui -> uids -> toPlainText().split(QRegularExpression("\\W"), QString::SkipEmptyParts).join("\n") + "\n");
//    ui -> uids -> blockSignals(false);
}
