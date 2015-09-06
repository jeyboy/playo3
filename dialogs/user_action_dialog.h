#ifndef USER_ACTION_DIALOG_H
#define USER_ACTION_DIALOG_H

#include <qdialog.h>
#include <qgridlayout.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>

namespace Ui { class UserActionDialog; }

enum FormInputType { image, text };

struct FormInput {
    FormInput(const QString & name, const QString & label, const QString & value = QString()) :
        label(label), name(name), value(value), ftype(text) {}

    FormInput(const QPixmap & value) : pict(value), ftype(image) {}

    QString label;
    QString name;
    QString value;
    QPixmap pict;
    FormInputType ftype;
};

class UserActionDialog : public QDialog {
    Q_OBJECT
public:
    explicit UserActionDialog(QWidget * parent = 0);
    ~UserActionDialog();

    void buildLoginForm(const QString & login_name = QStringLiteral("Login"), const QString & password_name = QStringLiteral("Password"));
    void buildCaptchaForm(const QPixmap & captcha_img);
    void buildForm(const QList<FormInput> & inputs);
    void extendForm(const QList<FormInput> & inputs);

    QString getValue(const QString & name);

private:
    QLineEdit * registerItem(QGridLayout * l, QString & name, QString & value);
    void createText(FormInput input, QGridLayout * l);
    void createImage(FormInput input, QGridLayout * l);

    void proceedInputs(const QList<FormInput> & inputs);
    void insertButtons();

    Ui::UserActionDialog * ui;
    QHash<QString, QLineEdit *> elements;
};

#endif // USER_ACTION_DIALOG_H
