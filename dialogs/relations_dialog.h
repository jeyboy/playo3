#ifndef RELATIONS_DIALOG_H
#define RELATIONS_DIALOG_H

#include <qdialog.h>
#include <qstringlistmodel.h>
#include <qhash.h>

#include "modules/core/web/interfaces/sociable.h"

namespace Ui { class RelationsDialog; }
using namespace Core;

class RelationsDialog : public QDialog {
    Q_OBJECT

    Ui::RelationsDialog * ui;
    QString uid, name;
    QStringListModel * friendModel, * groupModel;
    Web::Sociable * api;
public:
    explicit RelationsDialog(Web::Sociable * currApi, QWidget * parent = 0);
    ~RelationsDialog();

    inline QString getId() const { return uid; }
    inline QString getName() { return name; }

private slots:
    void on_cancelButton_clicked();
    void on_friendsList_activated(const QModelIndex & index);
    void on_groupsList_activated(const QModelIndex & index);

    void on_friendManually_clicked();
    void on_groupManually_clicked();
};

#endif // RELATIONS_DIALOG_H
