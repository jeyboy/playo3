#ifndef USER_ACTION_DIALOG_H
#define USER_ACTION_DIALOG_H

#include <qdialog.h>
#include <qgridlayout.h>

namespace Ui { class UserActionDialog; }

enum FormInputType { image, text };

struct FormInput {
    FormInput(FormInputType ftype, const QString & name, const QString & label, const QString & value) :
        label(label), name(name), value(value), ftype(ftype) {}

    QString label;
    QString name;
    QString value;
    FormInputType ftype;
};

class UserActionDialog : public QDialog {
    Q_OBJECT
public:
    explicit UserActionDialog(QWidget * parent = 0);
    ~UserActionDialog();

    void buildLoginForm(const QString & login_name = QStringLiteral("Login"), const QString & password_name = QStringLiteral("Password"));
    void buildCaptchaForm();
    void buildForm(QList<const FormInput &> inputs);
    void extendForm(QList<const FormInput &> inputs);

    QString getValue(const QString & name);
private:
    Ui::UserActionDialog * ui;
};

#endif // USER_ACTION_DIALOG_H
