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
    QString uid, name;
    Web::Sociable * api;
    QPushButton * default_btn;

    void prepareLinkablesList(const QList<Web::Linkable> & linkables, QListWidget * list);
    void done(int);
public:
    explicit RelationsDialog(ISource * currApi, QWidget * parent = 0);
    ~RelationsDialog();

    inline QString getId() const { return uid; }
    inline QString getName() { return name; }

private slots:
    void on_cancelButton_clicked();
    void on_friendsList_itemActivated(QListWidgetItem *item);
    void on_groupsList_itemActivated(QListWidgetItem *item);
    void on_groupByName_clicked();
    void on_friendByName_clicked();
    void on_friendById_clicked();
    void on_groupById_clicked();
    void onFocusChanged(QWidget * old, QWidget * now);
    void on_friendInList_clicked();
    void on_groupInList_clicked();
};

#endif // RELATIONS_DIALOG_H
