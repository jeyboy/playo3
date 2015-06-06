#ifndef TOOLBARDIALOG_H
#define TOOLBARDIALOG_H

#include <QDialog>

namespace Ui {
    class ToolbarDialog;
}

class ToolbarDialog : public QDialog {
    Q_OBJECT

public:
    explicit ToolbarDialog(QString title, QWidget * parent = 0);
    ~ToolbarDialog();

    QString getName();
    void setName(QString name);

private:
    Ui::ToolbarDialog * ui;
};

#endif // TOOLBARDIALOG_H
