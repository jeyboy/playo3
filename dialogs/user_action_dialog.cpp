#include "user_action_dialog.h"
#include "ui_user_action_dialog.h"

UserActionDialog::UserActionDialog(QWidget * parent) : QDialog(parent), ui(new Ui::UserActionDialog), finalized(false) {
    ui -> setupUi(this);
    new QHBoxLayout(this);
    createLayer();
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
    recreateLayer();
    proceedInputs(inputs);
}

void UserActionDialog::extendForm(const QList<FormInput> & inputs) {
    // need to check finalized and move buttons to the end of layout somehow on true state
    proceedInputs(inputs);
}

QString UserActionDialog::getValue(const QString & name) {
    QLineEdit * w = elements.value(name);
    if (w) return w -> text();
    return QString();
}

QLineEdit * UserActionDialog::registerItem(QString & name, QString & value) {
    QLineEdit * line = new QLineEdit(value, layer);
    elements.insert(name, line);

    return line;
}

void UserActionDialog::createText(FormInput input, QGridLayout * l) {
    if (input.label.isEmpty())
        insertElem(l, registerItem(input.name, input.value));
    else
        insertPair(l, new QLabel(input.label, layer), registerItem(input.name, input.value));
}
void UserActionDialog::createImage(FormInput input, QGridLayout * l) {
    QLabel * pict = new QLabel(layer);
    QPixmap pixmap = input.pict;

    if (pixmap.width() > MAX_PIXMAP_WIDTH) {
        int new_height = (MAX_PIXMAP_WIDTH / (float)pixmap.width()) * pixmap.height();
        pixmap = pixmap.scaled(MAX_PIXMAP_WIDTH, new_height, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    }

    pict -> setPixmap(pixmap);
    pict -> setMinimumHeight(pixmap.height());
    pict -> setMaximumHeight(pixmap.height());

    insertElem(l, pict);
    if (pixmap.height() > 90)
        l -> setRowMinimumHeight(l -> rowCount(), 35);
}

void UserActionDialog::createAction(FormInput input, QGridLayout * l) {
    QPushButton * button = new QPushButton(input.label, layer);
    actions.insert(button, input);
    connect(button, SIGNAL(clicked()), this, SLOT(actionRequired()));
    insertElem(l, button);
}

void UserActionDialog::proceedInputs(const QList<FormInput> & inputs) {
    QGridLayout * l = (QGridLayout *)layer -> layout();

    for(QList<FormInput>::ConstIterator input = inputs.cbegin(); input != inputs.cend(); input++) {
        switch((*input).ftype) {
            case text: createText(*input, l); break;
            case image: createImage(*input, l); break;
            case action: createAction(*input, l); break;
        }
    }
}

void UserActionDialog::insertButtons() {
    QGridLayout * l = (QGridLayout *)layer -> layout();

    QPushButton * cancelButton = new QPushButton(QStringLiteral("Cancel"), layer);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QPushButton * acceptButton = new QPushButton(QStringLiteral("Proceed"), layer);
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));

    insertPair(l, cancelButton, acceptButton);
    finalized = true;
}
