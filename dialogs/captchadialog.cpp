#include "captchadialog.h"
#include "ui_captchadialog.h"

CaptchaDialog::CaptchaDialog(QWidget * parent) :
    QDialog(parent), ui(new Ui::CaptchaDialog) {
    ui->setupUi(this);
}

CaptchaDialog::~CaptchaDialog() {
    delete ui;
}

//void CaptchaDialog::setImage(WebApi * api, QString url) {
//    ui -> captchaImg -> setPixmap(api -> openRemoteImage(url));
//}

void CaptchaDialog::clearText() {
    return ui -> captchaText -> setText("");
}

QString CaptchaDialog::captchaText() const {
    return ui -> captchaText -> text();
}

void CaptchaDialog::on_buttonBox_rejected() {
    ui -> captchaText -> setText("");
}
