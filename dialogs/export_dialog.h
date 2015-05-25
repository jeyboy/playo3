#ifndef EXPORT_DIALOG_H
#define EXPORT_DIALOG_H

#include <QDialog>
#include "modules/data_struct/model_interface.h"

namespace Ui {
class ExportDialog;
}

class ExportDialog : public QDialog {
    Q_OBJECT

public:
    explicit ExportDialog(IModel * parent);
    ~ExportDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ExportDialog * ui;
    IModel * mdl;
};

#endif // EXPORT_DIALOG_H
