#ifndef EXPORT_DIALOG_H
#define EXPORT_DIALOG_H

#include <QDialog>

namespace Ui {
    class ExportDialog;
}

class ExportDialog : public QDialog {
    Q_OBJECT

public:
    ExportDialog(QWidget * parent);
    ~ExportDialog();

    QStringList getUids();
private:
    Ui::ExportDialog * ui;
};

#endif // EXPORT_DIALOG_H
