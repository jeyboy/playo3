#ifndef RELATIONS_DIALOG_H
#define RELATIONS_DIALOG_H

#include <QDialog>
#include <QStringListModel>
#include <QHash>

#include "modules/web/web_api.h"

namespace Ui {
    class RelationsDialog;
}

class RelationsDialog : public QDialog {
    Q_OBJECT

public:
    explicit RelationsDialog(WebApi * currApi, QWidget * parent = 0);
    ~RelationsDialog();

    inline QString getId() const { return uid; }
    inline QString getName() { return name; }

private slots:
    void on_cancelButton_clicked();
    void on_friendsList_activated(const QModelIndex & index);
    void on_groupsList_activated(const QModelIndex & index);

    void on_friendManually_clicked();

    void on_groupManually_clicked();

private:
    Ui::RelationsDialog * ui;
    QString uid, name;
    QStringListModel * friendModel, * groupModel;
    WebApi * api;
};

#endif // RELATIONS_DIALOG_H
