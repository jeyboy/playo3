#ifndef USER_ACTION_DIALOG_H
#define USER_ACTION_DIALOG_H

#include <qdialog.h>
#include <qgridlayout.h>

namespace Ui { class UserActionDialog; }

enum FormInputType { image, text };

struct FormInput {
    FormInput(const QString & name, const QString & label, const QString & value) :
        label(label), name(name), value(value), ftype(text) {}

    FormInput(const QPixmap & value) : value(value), ftype(image) {}

    QString label;
    QString name;
    QVariant value;
    FormInputType ftype;
};

class UserActionDialog : public QDialog {
    Q_OBJECT
public:
    explicit UserActionDialog(QWidget * parent = 0);
    ~UserActionDialog();

    void buildLoginForm(const QString & login_name = QStringLiteral("Login"), const QString & password_name = QStringLiteral("Password"));
    void buildCaptchaForm(const QPixmap & captcha_img);
    void buildForm(const QList<const FormInput &> & inputs);
    void extendForm(const QList<const FormInput &> & inputs);

    QString getValue(const QString & name);

public slots:
    void approved();
    void canceled();
private:
    void proceedInputs(const QList<const FormInput &> & inputs);
    void insertButtons();

    Ui::UserActionDialog * ui;
    QHash<QString, QWidget *> elements;
};

#endif // USER_ACTION_DIALOG_H
