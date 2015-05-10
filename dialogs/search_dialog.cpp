#include "search_dialog.h"
#include "ui_search_dialog.h"
#include "dockbars.h"

#include <qdebug.h>

SearchDialog::SearchDialog(QWidget * parent) :
    QDialog(parent), ui(new Ui::SearchDialog)
{
    ui -> setupUi(this);
    connect(ui -> byTag, SIGNAL(clicked()), this, SLOT(on_nonByStyle_clicked()));
    connect(ui -> byTitle, SIGNAL(clicked()), this, SLOT(on_nonByStyle_clicked()));
    connect(ui -> byArtist, SIGNAL(clicked()), this, SLOT(on_nonByStyle_clicked()));
    connect(ui -> bySong, SIGNAL(clicked()), this, SLOT(on_nonByStyle_clicked()));

    QList<DockBar *> bars = Dockbars::instance() -> dockbars();
    QList<DockBar *>::Iterator it = bars.begin();

    for(; it != bars.end(); it++) {
        IView * v = Dockbars::instance() -> view(*it);
        if (v) {
            QListWidgetItem * item = new QListWidgetItem((*it) -> windowTitle(), ui -> tabsList);
            item -> setFlags(item -> flags() | Qt::ItemIsUserCheckable);
            item -> setCheckState(Qt::Checked);
            item -> setData(Qt::UserRole + 1, qVariantFromValue((void *) v));
            ui -> tabsList -> addItem(item);
        }
    }
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

    if (res.inTabs) {
        int count = ui -> tabsList -> count();

        for(int i = 0; i < count; i++) {
            QListWidgetItem * item = ui -> tabsList -> item(i);
            if (item -> checkState() == Qt::Checked)
                res.tabs.append(item -> data(Qt::UserRole + 1).value<void *>());
        }
    }

    qDebug() << "J" << res.tabs.length();

    return res;
}

void SearchDialog::on_addPredicate_clicked() {
    QString predicate;

    if (ui -> textPredicate -> isVisible())
        predicate = ui -> textPredicate -> text();
    else
        predicate = ui -> stylePredicate -> currentText();

    if (!predicate.isEmpty()) {
        if (ui -> predicates -> findItems(predicate, Qt::MatchFixedString).size() == 0)
            ui -> predicates -> addItem(predicate);
    }
}

void SearchDialog::on_byStyle_clicked() { ui -> textPredicate -> hide(); }
void SearchDialog::on_nonByStyle_clicked() { ui -> textPredicate -> show(); }

void SearchDialog::on_predicates_itemActivated(QListWidgetItem * item) {
    ui -> predicates -> removeItemWidget(item);
}

void SearchDialog::on_cancelButton_clicked() {
    reject();
}

void SearchDialog::on_acceptButton_clicked() {
    accept();
}
