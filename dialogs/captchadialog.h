#ifndef CAPTCHADIALOG_H
#define CAPTCHADIALOG_H

#include <QDialog>
#include <QPicture>
#include "modules/web/web_api.h"

namespace Ui {
class CaptchaDialog;
}

class CaptchaDialog : public QDialog {
    Q_OBJECT

public:
    explicit CaptchaDialog(QWidget * parent = 0);
    ~CaptchaDialog();
    void setImage(WebApi * api, QString url);
    inline void clearText() { ui -> captchaText -> setText(""); }
    QString captchaText() const;

private slots:
    inline void on_buttonBox_rejected() { clearText(); }

private:
    Ui::CaptchaDialog *ui;
};

#endif // CAPTCHADIALOG_H
