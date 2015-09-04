#include "user_action_dialog.h"
#include "ui_user_action_dialog.h"

UserActionDialog::UserActionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UserActionDialog)
{
    ui->setupUi(this);
}

UserActionDialog::~UserActionDialog()
{
    delete ui;
}
