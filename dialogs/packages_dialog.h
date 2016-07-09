#ifndef PACKAGES_DIALOG_H
#define PACKAGES_DIALOG_H

#include <qlistwidget.h>

#include "base_dialog.h"
#include "modules/core/interfaces/isource.h"

namespace Ui { class PackagesDialog; }
using namespace Core;

class PackagesDialog : public BaseDialog {
    Q_OBJECT

    Ui::PackagesDialog * ui;
    QString params, name;

    bool takeGenre(QString & genre);
public:
    explicit PackagesDialog(ISource * currApi, QWidget * parent = 0);
    ~PackagesDialog();

    inline QString getParams() const { return params; }
    inline QString getName() { return name; }

private slots:
    void on_cancelButton_clicked();
    void on_packagesList_itemActivated(QListWidgetItem *item);
};

#endif // PACKAGES_DIALOG_H
