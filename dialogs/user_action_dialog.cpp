#include "user_action_dialog.h"
#include "ui_user_action_dialog.h"

UserActionDialog::UserActionDialog(QWidget * parent) : BaseDialog(parent), ui(new Ui::UserActionDialog), window_title(0), finalized(false) {
    ui -> setupUi(this);
    new QHBoxLayout(this);
    createLayer();
}

UserActionDialog::~UserActionDialog() {
    delete ui;
}

void UserActionDialog::buildLoginWithCaptchaForm(const QPixmap & captcha_img, const QString & err, const QString & login_val, const QString & password_val, const QString & login_label, const QString & password_label) {
    inputs.clear();

    buildTitle(QLatin1String("Auth form"));

    if (!err.isEmpty()) buildErrFields(err);

    buildCaptchaFields(captcha_img);
    buildLoginFields(login_val, password_val, login_label, password_label);
    proceedForm(inputs);
}

void UserActionDialog::buildLoginForm(const QString & err, const QString & login_val, const QString & password_val, const QString & login_label, const QString & password_label) {
    inputs.clear();

    buildTitle(QLatin1String("Auth form"));

    if (!err.isEmpty()) buildErrFields(err);

    buildLoginFields(login_val, password_val, login_label, password_label);
    proceedForm(inputs);
}

void UserActionDialog::buildCaptchaForm(const QPixmap & captcha_img) {
    inputs.clear();

    buildTitle(QLatin1String("Captcha form"));

    buildCaptchaFields(captcha_img);
    proceedForm(inputs);
}

void UserActionDialog::buildToolbarButtonForm(const QString & name, const QString & path) {
    inputs.clear();

    buildTitle(QLatin1String("Toolbar Button form"));

    inputs << FormInput::createStr(name_key, QLatin1String("Name"), name);
    inputs << FormInput::createUrl(path_key, QLatin1String("Path"), path);
    proceedForm(inputs);
}

void UserActionDialog::buildGenreForm(const QString & genre) {
    inputs.clear();

    buildTitle(QLatin1String("Genre form"));

    inputs << FormInput::createStr(name_key, QLatin1String("Genre"), genre);
    proceedForm(inputs);
}

void UserActionDialog::buildToolbarForm(const QString & name) {
    inputs.clear();

    buildTitle(QLatin1String("Toolbar form"));

    inputs << FormInput::createStr(name_key, QLatin1String("Name"), name);
    proceedForm(inputs);
}

void UserActionDialog::buildPresetForm(const QString & name) {
    inputs.clear();

    buildTitle(QLatin1String("New preset form"));

    inputs << FormInput::createStr(name_key, QLatin1String("Name"), name);
    proceedForm(inputs);
}

void UserActionDialog::buildImportForm(const QString & text) {
    inputs.clear();

    buildTitle(QLatin1String("Import od IDs"));

    inputs << FormInput::createTxt(text_key, QLatin1String("Ids"), text);
    proceedForm(inputs);
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
    QPair<FormInputType, QWidget *> elem = elements.value(name);
    switch(elem.first) {
        case url:
        case password:
        case string: return ((QLineEdit *)elem.second) -> text();
        case text: return ((QPlainTextEdit *)elem.second) -> toPlainText();
        case list: return ((QComboBox *)elem.second) -> currentText();
        case checkbox: return ((QCheckBox *) elem.second) -> isChecked() ? QLatin1String("1") : QString();
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
        case password: {
            QLineEdit * pass_res = new QLineEdit(input.value, layer);
            pass_res -> setEchoMode(QLineEdit::Password);
            pass_res -> setInputMethodHints(Qt::ImhHiddenText | Qt::ImhNoPredictiveText | Qt::ImhNoAutoUppercase);
            res = pass_res;
        break;}
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
    QPushButton * button = new QPushButton(QLatin1String("Browse"), layer);
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

void UserActionDialog::createTitle(FormInput input, QGridLayout * l) {
    window_title = new QLabel(input.label, layer);
    window_title -> setStyleSheet(QLatin1String("font-weight: bold;"));
    insertElem(l, window_title);
}

void UserActionDialog::createMessage(FormInput input, QGridLayout * l) {
    QLabel * msg = new QLabel(input.label, layer);
    msg -> setStyleSheet(QLatin1String("color: red; font-weight: bold;"));
    msg -> setProperty("danger", true);
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
            case title: createTitle(*input, l); break;
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

    QPushButton * cancelButton = new QPushButton(QLatin1String("Cancel"), layer);
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QPushButton * acceptButton = new QPushButton(QLatin1String("Proceed"), layer);
    connect(acceptButton, SIGNAL(clicked()), this, SLOT(accept()));
    acceptButton -> setDefault(true);

    insertPair(l, cancelButton, acceptButton);
    finalized = true;
}
