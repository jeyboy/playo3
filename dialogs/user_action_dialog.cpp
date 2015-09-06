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
    QLineEdit * w = elements.value(name);
    if (w) return w -> text();
}

QLineEdit * UserActionDialog::registerItem(QGridLayout * l, QString & name, QString & value) {
    QLineEdit * line = new QLineEdit(value, l);
    elements.insert(name, line);

    return line;
}

void UserActionDialog::createText(const FormInput & input, QGridLayout * l) {
    if (input.label.isEmpty())
        l -> addWidget(registerItem(l, input.name, input.value), elements.size() - 1, 0, 0, 1, Qt::AlignCenter);
    else {
        l -> addWidget(new QLabel(input.label, f), elements.size(), 0, Qt::AlignCenter);
        l -> addWidget(registerItem(l, input.name, input.value), elements.size() - 1, 1, Qt::AlignCenter);
    }
}
void UserActionDialog::createImage(const FormInput & input, QGridLayout * l) {
    QLabel * pict = new QLabel(l);
    pict -> setPixmap(input.pict);

    l -> addWidget(pict, elements.size() - 1, 0, 0, 1, Qt::AlignCenter);
}

void UserActionDialog::proceedInputs(const QList<const FormInput &> & inputs) {
    QGridLayout * l = (QGridLayout *)layout();

    for(QList<const FormInput &>::ConstIterator input = inputs.cbegin(); input != inputs.cend(); input++) {
        switch((*input).ftype) {
            case text: createText(*input, l); break;
            case image: createImage(*input, l); break;
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
