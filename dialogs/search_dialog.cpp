#include "search_dialog.h"
#include "ui_search_dialog.h"

SearchDialog::SearchDialog(QWidget * parent) :
    QDialog(parent), ui(new Ui::SearchDialog)
{
    ui -> setupUi(this);
    connect(ui -> byTag, SIGNAL(clicked()), this, SLOT(on_nonByStyle_clicked()));
    connect(ui -> byTitle, SIGNAL(clicked()), this, SLOT(on_nonByStyle_clicked()));
    connect(ui -> byArtist, SIGNAL(clicked()), this, SLOT(on_nonByStyle_clicked()));
    connect(ui -> bySong, SIGNAL(clicked()), this, SLOT(on_nonByStyle_clicked()));
}

SearchDialog::~SearchDialog() {
    delete ui;
}

SearchSettings SearchDialog::params() {
    SearchSettings res(ui -> inVk -> isChecked(), ui -> inSc -> isChecked(), ui -> inOther -> isChecked(),
                       ui -> inTabs -> isChecked(), ui -> inComputer -> isChecked());
    int count = ui -> predicates -> count();

    for(int i = 0; i < count; i++)
        res.predicates.append(ui -> predicates -> item(i) -> text());

    if (ui -> byTitle -> isChecked())
        res.type = ::title;
    else if (ui -> byArtist -> isChecked())
        res.type = ::artist;
    else if (ui -> bySong -> isChecked())
        res.type = ::song;
    else if (ui -> byStyle -> isChecked())
        res.type = ::style;
    else
        res.type = ::tag;

    res.popular = ui -> byPopular -> isChecked();

    return res;
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
