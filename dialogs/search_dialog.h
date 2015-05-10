#ifndef SEARCH_DIALOG_H
#define SEARCH_DIALOG_H

#include <QDialog>
#include <QListWidgetItem>

#include "modules/data_struct/search/search_settings.h"

namespace Ui {
    class SearchDialog;
}

class SearchDialog : public QDialog {
    Q_OBJECT

public:
    explicit SearchDialog(QWidget * parent = 0);
    ~SearchDialog();

    SearchSettings params();

private slots:
    void on_addPredicate_clicked();

    void on_byStyle_clicked();
    void on_nonByStyle_clicked();

    void on_predicates_itemActivated(QListWidgetItem * item);

    void on_cancelButton_clicked();

    void on_acceptButton_clicked();

private:
    Ui::SearchDialog * ui;
};

#endif // SEARCH_DIALOG_H