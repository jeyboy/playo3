#ifndef SEARCH_DIALOG_H
#define SEARCH_DIALOG_H

#include "base_dialog.h"
#include "modules/controls/search_configurator.h"

namespace Ui { class SearchDialog; }

class SearchDialog : public BaseDialog {
    Q_OBJECT

    Ui::SearchDialog * ui;
    SearchConfigurator * configurator;
public:
    explicit SearchDialog(QWidget * parent = 0);
    ~SearchDialog();

    SearchSettings params();
private slots:
    void on_cancelButton_clicked();
    void on_acceptButton_clicked();
};

#endif // SEARCH_DIALOG_H
