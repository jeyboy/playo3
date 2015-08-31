#include "captchadialog.h"
#include "ui_captchadialog.h"

CaptchaDialog::CaptchaDialog(QWidget * parent) :
    QDialog(parent), ui(new Ui::CaptchaDialog) {
    ui -> setupUi(this);
    ui -> customAction -> setVisible(false);
}

CaptchaDialog::~CaptchaDialog() {
    delete ui;
}

void CaptchaDialog::setDescription(const QString & text) {
    ui -> captchaImg -> setText(text);
}

void CaptchaDialog::setActionText(const QString & text, QObject * receiver, const char * slot) {
    if (ui -> customAction -> isVisible())
        disconnect(ui -> customAction);

    if (receiver) {
        ui -> customAction ->  setText(text);
        connect(ui -> customAction, SIGNAL(clicked()), receiver, slot);
        ui -> customAction -> setVisible(true);
    } else ui -> customAction -> setVisible(false);
}

void CaptchaDialog::setImage(QPixmap pixmap) {
    ui -> captchaImg -> setPixmap(pixmap);
}

void CaptchaDialog::clearText() {
    ui -> captchaText -> setText("");
}

QString CaptchaDialog::captchaText() const {
    return ui -> captchaText -> text();
}
