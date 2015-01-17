#ifndef CAPTCHADIALOG_H
#define CAPTCHADIALOG_H

#include <QDialog>
#include <QPicture>
//#include "web/web_api.h"

namespace Ui {
class CaptchaDialog;
}

class CaptchaDialog : public QDialog {
    Q_OBJECT

public:
    explicit CaptchaDialog(QWidget *parent = 0);
    ~CaptchaDialog();
//    void setImage(WebApi * api, QString url);
    void clearText();
    QString captchaText() const;

private slots:
    void on_buttonBox_rejected();

private:
    Ui::CaptchaDialog *ui;
};

#endif // CAPTCHADIALOG_H
