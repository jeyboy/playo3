#ifndef RELATIONS_DIALOG_H
#define RELATIONS_DIALOG_H

#include <qlistwidget.h>

#include "base_dialog.h"
#include "modules/core/interfaces/isource.h"
#include "modules/core/web/interfaces/sociable/sociable.h"

#define EMPTY_RESULT_MESSAGE LSTR("Did not found anything")

namespace Ui { class RelationsDialog; }
using namespace Core;

class RelationsDialog : public BaseDialog {
    Q_OBJECT

    Ui::RelationsDialog * ui;
    Web::Sociable * api;

    void prepareLinkablesList(const QList<Web::Linkable> & linkables, QListWidget * list);
    void done(int);
public:
    explicit RelationsDialog(ISource * currApi, QWidget * parent = 0);
    ~RelationsDialog();
signals:
    void relationTabCreationRequired(const QString & rel_name, const QString & id);
private slots:
    void on_friendsList_itemActivated(QListWidgetItem *item);
    void on_groupsList_itemActivated(QListWidgetItem *item);
    void on_groupByName_clicked();
    void on_friendByName_clicked();
    void on_friendById_clicked();
    void on_groupById_clicked();
    void on_closeButton_clicked();
    void on_friendPredicate_textChanged(const QString &arg1);
    void on_groupPredicate_textChanged(const QString &arg1);
};

#endif // RELATIONS_DIALOG_H
