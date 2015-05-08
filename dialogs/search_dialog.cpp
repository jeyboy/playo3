#include "search_dialog.h"
#include "ui_search_dialog.h"
#include <qdebug.h>

SearchDialog::SearchDialog(QWidget * parent) :
    QDialog(parent), ui(new Ui::SearchDialog)
{
    ui -> setupUi(this);
    connect(ui -> byUser, SIGNAL(clicked()), this, SLOT(on_nonByStyle_clicked()));
    connect(ui -> byGroup, SIGNAL(clicked()), this, SLOT(on_nonByStyle_clicked()));
    connect(ui -> byTag, SIGNAL(clicked()), this, SLOT(on_nonByStyle_clicked()));
    connect(ui -> byTitle, SIGNAL(clicked()), this, SLOT(on_nonByStyle_clicked()));

    connect(ui -> inVk, SIGNAL(clicked()), this, SLOT(on_inPortals_clicked()));
    connect(ui -> inSc, SIGNAL(clicked()), this, SLOT(on_inPortals_clicked()));

    on_inPortals_clicked();
}

SearchDialog::~SearchDialog() {
    delete ui;
}

void SearchDialog::on_addPredicate_clicked() {
    QString predicate;

    if (ui -> textPredicate -> isVisible())
        predicate = ui -> textPredicate -> text();
    else
        predicate = ui -> stylePredicate -> currentText();

    if (!predicate.isEmpty())
        ui -> predicates -> addItem(predicate);
}

void SearchDialog::on_byStyle_clicked() { ui -> textPredicate -> hide(); }
void SearchDialog::on_nonByStyle_clicked() { ui -> textPredicate -> show(); }

void SearchDialog::on_inPortals_clicked() {
    bool selected = ui -> inVk -> isChecked() || ui -> inSc -> isChecked();

    ui -> byGroup -> setDisabled(!selected);
    ui -> byUser -> setDisabled(!selected);

    if (!selected && (ui -> byGroup -> isChecked() || ui -> byUser -> isChecked()))
        ui -> byTitle -> setChecked(true);
}
