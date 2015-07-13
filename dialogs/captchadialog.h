#ifndef CAPTCHADIALOG_H
#define CAPTCHADIALOG_H

#include <qdialog.h>

namespace Ui { class CaptchaDialog; }

class WebApi;

class CaptchaDialog : public QDialog {
    Q_OBJECT

public:
    explicit CaptchaDialog(QWidget * parent = 0);
    ~CaptchaDialog();
    void setImage(QPixmap pixmap);
    void clearText();
    QString captchaText() const;

private slots:
    inline void on_buttonBox_rejected() { clearText(); }

private:
    Ui::CaptchaDialog * ui;
};

#endif // CAPTCHADIALOG_H
