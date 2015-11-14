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

void UserActionDialog::buildLoginForm(const QString & err, const QString & login_label, const QString & password_label) {
    QList<FormInput> inputs;

    if (!err.isEmpty())
        inputs << FormInput::createMessage(err);

    inputs << FormInput::createStr(login_key, login_label);
    inputs << FormInput::createStr(pass_key, password_label);
    buildForm(inputs, QStringLiteral("Auth form"));
}

void UserActionDialog::buildCaptchaForm(const QPixmap & captcha_img) {
    QList<FormInput> inputs;
    inputs << FormInput(captcha_img);
    inputs << FormInput::createStr(captcha_key, QStringLiteral("Captcha value"));
    buildForm(inputs, QStringLiteral("Captcha form"));
}

void UserActionDialog::buildToolbarButtonForm(const QString & name, const QString & path) {
    QList<FormInput> inputs;
    inputs << FormInput::createStr(name_key, QStringLiteral("Name"), name);
    inputs << FormInput::createUrl(path_key, QStringLiteral("Path"), path);
    buildForm(inputs, QStringLiteral("Toolbar Button form"));
}

void UserActionDialog::buildToolbarForm(const QString & name) {
    QList<FormInput> inputs;
    inputs << FormInput::createStr(name_key, QStringLiteral("Name"), name);
    buildForm(inputs, QStringLiteral("Toolbar form"));
}

void UserActionDialog::buildPresetForm(const QString & name) {
    QList<FormInput> inputs;
    inputs << FormInput::createStr(name_key, QStringLiteral("Name"), name);
    buildForm(inputs, QStringLiteral("New preset form"));
}

void UserActionDialog::buildImportForm(const QString & text) {
    QList<FormInput> inputs;
    inputs << FormInput::createTxt(text_key, QStringLiteral("Ids"), text);
    buildForm(inputs, QStringLiteral("Import od IDs"));
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
        case url:
        case string: return ((QLineEdit *)elem.second) -> text();
        case text: return ((QPlainTextEdit *)elem.second) -> toPlainText();
        case list: return ((QComboBox *)elem.second) -> currentText();
        case checkbox: return ((QCheckBox *) elem.second) -> isChecked() ? QStringLiteral("1") : QString();
        default: return QString();
    }
}

QWidget * UserActionDialog::registerItem(FormInput & input) {
    QWidget * res;

    switch(input.ftype) {
        case list: {
            res = new QComboBox(layer);
            ((QComboBox *)res) -> insertItems(0, input.opts);
            ((QComboBox *)res) -> setCurrentText(input.value);
        break;}
        case checkbox: {
            res = new QCheckBox(input.label, layer);
            ((QCheckBox *)res) -> setChecked(!input.value.isEmpty());
        }
        case text: {res = new QPlainTextEdit(input.value, layer); break;}
        default: res = new QLineEdit(input.value, layer);
    }
    elements.insert(input.name, QPair<FormInputType, QWidget *>(input.ftype, res));

    return res;
}

void UserActionDialog::createElement(FormInput input, QGridLayout * l) {
    if (input.label.isEmpty())
        insertElem(l, registerItem(input));
    else
        insertPair(l, new QLabel(input.label, layer), registerItem(input));
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

void UserActionDialog::createMessage(FormInput input, QGridLayout * l) {
    QLabel * msg = new QLabel(input.label, layer);
    msg -> setStyleSheet(QStringLiteral("color: red; font-weight: bold;"));
    insertElem(l, msg);
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
            case url: createUrl(*input, l); break;
            case action: createAction(*input, l); break;
            case image: createImage(*input, l); break;
            case message: createMessage(*input, l); break;
            default: createElement(*input, l);
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
