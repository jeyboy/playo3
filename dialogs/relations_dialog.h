#ifndef RELATIONS_DIALOG_H
#define RELATIONS_DIALOG_H

#include <qdialog.h>
#include <qlistwidget.h>

#include "modules/core/web/interfaces/sociable.h"

namespace Ui { class RelationsDialog; }
using namespace Core;

class RelationsDialog : public QDialog {
    Q_OBJECT

    Ui::RelationsDialog * ui;
    QString uid, name;
    Web::Sociable * api;

    void prepareLinkablesList(QHash<QString, Web::Linkable> linkables, QListWidget * list);
public:
    explicit RelationsDialog(Web::Sociable * currApi, QWidget * parent = 0);
    ~RelationsDialog();

    inline QString getId() const { return uid; }
    inline QString getName() { return name; }

private slots:
    void on_cancelButton_clicked();
    void on_friendManually_clicked();
    void on_groupManually_clicked();
    void on_friendsList_itemActivated(QListWidgetItem *item);
    void on_groupsList_itemActivated(QListWidgetItem *item);
};

#endif // RELATIONS_DIALOG_H
