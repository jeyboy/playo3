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

void UserActionDialog::buildLoginForm(const QString & login_label, const QString & password_label) {
    QList<FormInput> inputs;
    inputs << FormInput(login_key, true, login_label);
    inputs << FormInput(pass_key, true, password_label);
    buildForm(inputs, QStringLiteral("Auth form"));
}

void UserActionDialog::buildCaptchaForm(const QPixmap & captcha_img) {
    QList<FormInput> inputs;
    inputs << FormInput(captcha_img);
    inputs << FormInput(captcha_key, true, QStringLiteral("Captcha value"));
    buildForm(inputs, QStringLiteral("Captcha form"));
}

void UserActionDialog::buildToolbarButtonForm(const QString & name, const QString & path) {
    QList<FormInput> inputs;
    inputs << FormInput(name_key, true, QStringLiteral("Name"), name);
    inputs << FormInput(path_key, QStringLiteral("Path"), path);
    buildForm(inputs, QStringLiteral("Toolbar Button form"));
}

void UserActionDialog::buildForm(const QList<FormInput> & inputs, const QString & title) {
    setWindowTitle(title);
    recreateLayer();
    proceedInputs(inputs);
}

void UserActionDialog::extendForm(const QList<FormInput> & inputs) {
    // need to check finalized and move buttons to the end of layout somehow on true state
    proceedInputs(inputs);
}

QString UserActionDialog::getValue(const QString & name) {
    QPair<FormInputType, QWidget *> elem = elements.value(name);
    switch(elem.first) {
        case string: return ((QLineEdit *)elem.second) -> text();
        case text: return ((QPlainTextEdit *)elem.second) -> toPlainText();
        default: return QString();
    }
}

QWidget * UserActionDialog::registerItem(FormInput & input) {
    QWidget * res;

    switch(input.ftype) {
        case text: {res = new QPlainTextEdit(input.value, layer); break;}
        default: res = new QLineEdit(input.value, layer);
    }
    elements.insert(input.name, QPair<FormInputType, QWidget *>(input.ftype, res));

    return res;
}

void UserActionDialog::createUrl(FormInput input, QGridLayout * l) {
    QPushButton * button = new QPushButton(QStringLiteral("Browse"), layer);
    button -> setProperty("owner_key", input.name);
    connect(button, SIGNAL(clicked(bool)), this, SLOT(browseClicked()));

    if (input.label.isEmpty())
        insertPair(l, button, registerItem(input));
    else
        insertPairPlus(l, new QLabel(input.label, layer), registerItem(input), button);
}

void UserActionDialog::createString(FormInput input, QGridLayout * l) {
    if (input.label.isEmpty())
        insertElem(l, registerItem(input));
    else
        insertPair(l, new QLabel(input.label, layer), registerItem(input));
}

void UserActionDialog::createText(FormInput input, QGridLayout * l) {
    if (input.label.isEmpty())
        insertElem(l, registerItem(input));
    else
        insertPair(l, new QLabel(input.label, layer), registerItem(input));
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
            case string: createString(*input, l); break;
            case action: createAction(*input, l); break;
            case text: createText(*input, l); break;
            case image: createImage(*input, l); break;
            case url: createUrl(*input, l); break;
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
