#ifndef SEARCH_DIALOG_H
#define SEARCH_DIALOG_H

#include <qdialog.h>
#include <qlistwidget.h>

#include "modules/models/service/search_settings.h"

namespace Ui { class SearchDialog; }

class SearchDialog : public QDialog {
    Q_OBJECT
public:
    explicit SearchDialog(QWidget * parent = 0);
    ~SearchDialog();

    SearchSettings params();

private slots:
    void on_addPredicate_clicked();

    void on_cancelButton_clicked();

    void on_acceptButton_clicked();

    void on_textPredicates_itemActivated(QListWidgetItem * item);

    void on_stylePredicates_itemActivated(QListWidgetItem * item);

    void on_addStylePredicate_clicked();

    void on_driveList_itemClicked(QListWidgetItem * item);

    void on_inComputer_toggled(bool checked);

    void on_inTabs_toggled(bool checked);

    void on_tabsList_itemClicked(QListWidgetItem *item);

    void on_inSites_toggled(bool checked);

    void on_sitesList_itemClicked(QListWidgetItem *item);

private:
    Ui::SearchDialog * ui;
};

#endif // SEARCH_DIALOG_H
