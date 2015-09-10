#ifndef USER_ACTION_DIALOG_H
#define USER_ACTION_DIALOG_H

#include <qdialog.h>
#include <qgridlayout.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>

#include <qdebug.h>

namespace Ui { class UserActionDialog; }

enum FormInputType { image, text, action };

struct FormInput {
    inline FormInput() {}

    inline FormInput(const QString & name, const QString & label, const QString & value = QString()) :
        label(label), name(name), value(value), ftype(text) {}

    inline FormInput(const QPixmap & value) : pict(value), ftype(image) {}

    inline FormInput(const QString & text, const QString & value, QObject * receiver, const char * slot) : label(text),
        value(value), obj(receiver), slot(slot), ftype(action) {}

    QString label;
    QString name;
    QString value;
    QPixmap pict;

    QObject * obj;
    const char * slot;

    FormInputType ftype;
};

#define MAX_PIXMAP_WIDTH 370

class UserActionDialog : public QDialog {
    Q_OBJECT
public:
    const QString captcha_key = QStringLiteral("captcha");

    const QString login_key = QStringLiteral("login");
    const QString pass_key = QStringLiteral("password");

    explicit UserActionDialog(QWidget * parent = 0);
    ~UserActionDialog();

    void buildLoginForm(const QString & login_name = QStringLiteral("Login"), const QString & password_name = QStringLiteral("Password"));
    void buildCaptchaForm(const QPixmap & captcha_img);
    void buildForm(const QList<FormInput> & inputs);
    void extendForm(const QList<FormInput> & inputs);

    QString getValue(const QString & name);

public slots:
    inline int exec() {
        if (!finalized) {
            adjustSize();
            insertButtons();
        }
        return QDialog::exec();
    }

protected slots:
    inline void actionRequired() {
        FormInput input = actions.value(sender());
        QMetaObject::invokeMethod(input.obj, input.slot, Qt::AutoConnection, Q_ARG(QString &, input.value));
    }

private:
    void insertElem(QGridLayout * l, QWidget * w) {
        l -> addWidget(w, l -> rowCount(), 0, 1, 2, Qt::AlignCenter);
    }

    void insertPair(QGridLayout * l, QWidget * w1, QWidget * w2) {
        int row = l -> rowCount();
        l -> addWidget(w1, row, 0);
        l -> addWidget(w2, row, 1);
    }

    inline void createLayer() {
        QHBoxLayout * l = (QHBoxLayout *)layout();
        l -> addWidget((layer = new QWidget(this)), 1);
        new QGridLayout(layer);
        layout() -> setContentsMargins(0,0,0,0);
    }
    inline void recreateLayer() {
        delete layer;
        elements.clear();
        actions.clear();
        finalized = false;
        createLayer();
    }

    QLineEdit * registerItem(QString & name, QString & value);
    void createText(FormInput input, QGridLayout * l);
    void createImage(FormInput input, QGridLayout * l);
    void createAction(FormInput input, QGridLayout * l);

    void proceedInputs(const QList<FormInput> & inputs);
    void insertButtons();

    Ui::UserActionDialog * ui;
    QHash<QString, QLineEdit *> elements;
    QHash<QObject *, FormInput> actions;
    QWidget * layer;
    bool finalized;
};

#endif // USER_ACTION_DIALOG_H
