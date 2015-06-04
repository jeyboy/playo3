#include "captchadialog.h"
#include "ui_captchadialog.h"

CaptchaDialog::CaptchaDialog(QWidget * parent) :
    QDialog(parent), ui(new Ui::CaptchaDialog) {
    ui -> setupUi(this);
}

CaptchaDialog::~CaptchaDialog() {
    delete ui;
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
