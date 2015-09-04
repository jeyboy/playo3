#ifndef USER_ACTION_DIALOG_H
#define USER_ACTION_DIALOG_H

#include <QDialog>

namespace Ui {
class UserActionDialog;
}

class UserActionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserActionDialog(QWidget *parent = 0);
    ~UserActionDialog();

private:
    Ui::UserActionDialog *ui;
};

#endif // USER_ACTION_DIALOG_H
