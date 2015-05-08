#ifndef SEARCH_DIALOG_H
#define SEARCH_DIALOG_H

#include <QDialog>

namespace Ui {
    class SearchDialog;
}

class SearchDialog : public QDialog {
    Q_OBJECT

public:
    explicit SearchDialog(QWidget * parent = 0);
    ~SearchDialog();

private slots:
    void on_addPredicate_clicked();

    inline void on_byTag_clicked() { ui -> textPredicate -> hide(); }
    inline void on_nonByTag_clicked() { ui -> textPredicate -> show(); }

    void on_byStyle_clicked();

private:
    Ui::SearchDialog * ui;
};

#endif // SEARCH_DIALOG_H
