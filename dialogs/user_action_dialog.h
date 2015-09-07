#ifndef USER_ACTION_DIALOG_H
#define USER_ACTION_DIALOG_H

#include <qdialog.h>
#include <qgridlayout.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>

namespace Ui { class UserActionDialog; }

enum FormInputType { image, text, action };

struct FormInput {
    FormInput(const QString & name, const QString & label, const QVariant & value = QString()) :
        label(label), name(name), value(value), ftype(text) {}

    FormInput(const QPixmap & value) : pict(value), ftype(image) {}

    FormInput(const QString & text, const QVariant & value, QObject * receiver, const char * slot) : label(text),
        value(value), obj(receiver), slot(slot), ftype(action) {}

    QString label;
    QString name;
    QVariant value;
    QPixmap pict;

    QObject * obj;
    const char * slot;

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

protected slots:
    inline void actionRequired() {
        FormInput input = actions.value(sender());
        QMetaObject::invokeMethod(input.obj, input.slot, Qt::AutoConnection, Q_ARG(QVariant &, input.value));
    }

private:
    QLineEdit * registerItem(QGridLayout * l, QString & name, QString & value);
    void createText(FormInput input, QGridLayout * l);
    void createImage(FormInput input, QGridLayout * l);
    void createAction(FormInput input, QGridLayout * l);

    void proceedInputs(const QList<FormInput> & inputs);
    void insertButtons();

    Ui::UserActionDialog * ui;
    QHash<QString, QLineEdit *> elements;
    QHash<QPushButton *, FormInput> actions;
};

#endif // USER_ACTION_DIALOG_H
