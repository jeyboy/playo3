#include "search_dialog.h"
#include "ui_search_dialog.h"

SearchDialog::SearchDialog(QWidget * parent) :
    QDialog(parent), ui(new Ui::SearchDialog)
{
    ui -> setupUi(this);
    connect(ui -> byUser, SIGNAL(clicked()), this, SLOT(on_nonByStyle_clicked()));
    connect(ui -> byGroup, SIGNAL(clicked()), this, SLOT(on_nonByStyle_clicked()));
    connect(ui -> byTag, SIGNAL(clicked()), this, SLOT(on_nonByStyle_clicked()));
    connect(ui -> byTitle, SIGNAL(clicked()), this, SLOT(on_nonByStyle_clicked()));
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
