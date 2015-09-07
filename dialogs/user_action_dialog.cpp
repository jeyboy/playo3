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
    QList<FormInput> inputs;
    inputs << FormInput(QStringLiteral("login"), login_name);
    inputs << FormInput(QStringLiteral("password"), password_name);
    buildForm(inputs);
}

void UserActionDialog::buildCaptchaForm(const QPixmap & captcha_img) {
    QList<FormInput> inputs;
    inputs << FormInput(captcha_img);
    inputs << FormInput(QStringLiteral("captcha"), QStringLiteral("Captcha value"));
    buildForm(inputs);
}

void UserActionDialog::buildForm(const QList<FormInput> & inputs) {
    delete layout();
    elements.clear();
    actions.clear();
    new QGridLayout(this);
    proceedInputs(inputs);
}

void UserActionDialog::extendForm(const QList<FormInput> & inputs) {
    proceedInputs(inputs);
}

QString UserActionDialog::getValue(const QString & name) {
    QLineEdit * w = elements.value(name);
    if (w) return w -> text();
    return QString();
}

QLineEdit * UserActionDialog::registerItem(QGridLayout * l, QString & name, QString & value) {
    QLineEdit * line = new QLineEdit(value, (QWidget *)l);
    elements.insert(name, line);

    return line;
}

void UserActionDialog::createText(FormInput input, QGridLayout * l) {
    if (input.label.isEmpty())
        l -> addWidget(registerItem(l, input.name, input.value), elements.size() - 1, 0, 0, 1, Qt::AlignCenter);
    else {
        l -> addWidget(new QLabel(input.label, (QWidget *)l), elements.size(), 0, Qt::AlignCenter);
        l -> addWidget(registerItem(l, input.name, input.value), elements.size() - 1, 1, Qt::AlignCenter);
    }
}
void UserActionDialog::createImage(FormInput input, QGridLayout * l) {
    QLabel * pict = new QLabel((QWidget *)l);
    pict -> setPixmap(input.pict);

    l -> addWidget(pict, elements.size() - 1, 0, 0, 1, Qt::AlignCenter);
}

void UserActionDialog::createAction(FormInput input, QGridLayout * l) {
    QPushButton * button = new QPushButton(input.label, (QWidget *)l);
    actions.insert(button, input);
    connect(button, SIGNAL(clicked()), this, SLOT(actionRequired()));
    l -> addWidget(button, elements.size() - 1, 0, 0, 1, Qt::AlignCenter);
}

void UserActionDialog::proceedInputs(const QList<FormInput> & inputs) {
    QGridLayout * l = (QGridLayout *)layout();

    for(QList<FormInput>::ConstIterator input = inputs.cbegin(); input != inputs.cend(); input++) {
        switch((*input).ftype) {
            case text: createText(*input, l); break;
            case image: createImage(*input, l); break;
            case action: createAction(*input, l); break;
        }
    }
}

void UserActionDialog::insertButtons() {
    QGridLayout * l = (QGridLayout *)layout();

    QPushButton * cancelButton = new QPushButton((QWidget *)l);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));
    l -> addWidget(cancelButton, elements.size(), 0);

    QPushButton * acceptButton = new QPushButton((QWidget *)l);
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
    l -> addWidget(acceptButton, elements.size(), 1);
}
