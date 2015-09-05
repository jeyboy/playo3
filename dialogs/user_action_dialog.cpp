#include "user_action_dialog.h"
#include "ui_user_action_dialog.h"

UserActionDialog::UserActionDialog(QWidget * parent) : QDialog(parent), ui(new Ui::UserActionDialog) {
    ui -> setupUi(this);

    new QGridLayout(this);
}

UserActionDialog::~UserActionDialog() {
    delete ui;
}

void UserActionDialog::buildLoginForm(const QString & login_name, const QString & password_name) {
    QList<const FormInput &> inputs;
    inputs << FormInput(text, QStringLiteral("login"), login_name);
    inputs << FormInput(text, QStringLiteral("password"), password_name);
}

void UserActionDialog::buildCaptchaForm() {

}

void UserActionDialog::buildForm(QList<const FormInput &> inputs) {
     delete layout();

     QGridLayout * l = new QGridLayout(this);


}

void UserActionDialog::extendForm(QList<const FormInput &> inputs) {
     delete layout();

     QGridLayout * l = new QGridLayout(this);


}

QString UserActionDialog::getValue(const QString & name) {

}
