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
    inputs << FormInput(QStringLiteral("login"), login_name);
    inputs << FormInput(QStringLiteral("password"), password_name);
    buildForm(inputs);
}

void UserActionDialog::buildCaptchaForm(const QPixmap & captcha_img) {
    QList<const FormInput &> inputs;
    inputs << FormInput(captcha_img);
    inputs << FormInput(QStringLiteral("captcha"), QStringLiteral("Captcha value"));
    buildForm(inputs);
}

void UserActionDialog::buildForm(const QList<const FormInput &> & inputs) {
    delete layout();
    elements.clear();
    new QGridLayout(this);
    proceedInputs(inputs);
}

void UserActionDialog::extendForm(const QList<const FormInput &> & inputs) {
    proceedInputs(inputs);
}

QString UserActionDialog::getValue(const QString & name) {
    QWidget * w = elements.value(name);
//    if (w) return
}

void UserActionDialog::proceedInputs(const QList<const FormInput &> & inputs) {
    for(QList<const FormInput &>::ConstIterator input = inputs.cbegin(); input != inputs.cend(); input++) {
        switch((*input).ftype) {
            case text: createText(); break;
            case image: break;
        }
    }
}

void UserActionDialog::insertButtons() {
    QGridLayout * l = (QGridLayout *)layout();

    QPushButton * cancelButton = new QPushButton(l);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(canceled()));
    l -> addWidget(cancelButton, elements.size(), 0);

    QPushButton * acceptButton = new QPushButton(l);
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(approved()));
    l -> addWidget(acceptButton, elements.size(), 1);
}
