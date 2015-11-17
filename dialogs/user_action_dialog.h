#ifndef USER_ACTION_DIALOG_H
#define USER_ACTION_DIALOG_H

#include <qfiledialog.h>
//#include <qsyntaxhighlighter.h>

#include <qdialog.h>
#include <qgridlayout.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qplaintextedit.h>
#include <QTextBlock>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qpair.h>

#include "modules/core/misc/thread_utils.h"

namespace Ui { class UserActionDialog; }

enum FormInputType { image, string, text, action, url, list, checkbox, message };

//class IdHighlighter : public QSyntaxHighlighter {
//    Q_OBJECT
//public:
//    IdHighlighter(QTextDocument * parent) : QSyntaxHighlighter(parent) {}

//    void highlightBlock(const QString & text) {
//        QTextCharFormat sep;
//        sep.setFontWeight(QFont::Bold);
//        sep.setForeground(Qt::darkRed);
//        setFormat(0, text.length(), sep);

//        QTextCharFormat socialId;
//        socialId.setForeground(Qt::darkBlue);
//        QString pattern = QStringLiteral("\\b(vk|sc)[\\d\\-_]+\\b");

//        QRegExp expression2(pattern);
//        int index = text.indexOf(expression2);
//        while (index >= 0) {
//            int length = expression2.matchedLength();
//            setFormat(index, length, socialId);
//            index = text.indexOf(expression2, index + length);
//        }
//    }
//};

// highlighter = new IdHighlighter(ui -> uids -> document());
//return ui -> uids -> toPlainText().split(QRegularExpression(QStringLiteral("\\W")), QString::SkipEmptyParts);


struct FormInput {
    inline FormInput() {}

    static inline FormInput createMessage(const QString & label) {
        return FormInput(message, QString(), label);
    }

    static inline FormInput createUrl(const QString & name, const QString & label, const QString & value = QString()) {
        return FormInput(url, name, label, value);
    }

    static inline FormInput createStr(const QString & name, const QString & label, const QString & value = QString()) {
        return FormInput(string, name, label, value);
    }

    static inline FormInput createTxt(const QString & name, const QString & label, const QString & value = QString()) {
        return FormInput(text, name, label, value);
    }

    static inline FormInput createSelect(const QString & name, const QString & label, const QStringList & options, const QString & value = QString()) {
        return FormInput(list, name, label, options, value);
    }

    static inline FormInput createCheckbox(const QString & name, const QString & label, const QString & value = QString()) {
        return FormInput(checkbox, name, label, value);
    }

    inline FormInput(FormInputType etype, const QString & name, const QString & label, const QString & value = QString()) :
        label(label), name(name), value(value), ftype(etype) {}

    inline FormInput(FormInputType etype, const QString & name, const QString & label, const QStringList & options, const QString & value) :
        label(label), name(name), value(value), opts(options), ftype(etype) {}

    inline FormInput(const QPixmap & value) : pict(value), ftype(image) {}

    inline FormInput(const QString & text, const QString & value, QObject * receiver, const char * slot) : label(text),
        value(value), obj(receiver), slot(slot), ftype(action) {}

    QString label;
    QString name;
    QString value;
    QPixmap pict;
    QStringList opts;

    QObject * obj;
    const char * slot;

    FormInputType ftype;
};

#define MAX_PIXMAP_WIDTH 370

class UserActionDialog : public QDialog {
    Q_OBJECT
public:
    const QString name_key = QStringLiteral("name");
    const QString path_key = QStringLiteral("path");

    const QString text_key = QStringLiteral("text");

    const QString captcha_key = QStringLiteral("captcha");

    const QString login_key = QStringLiteral("login");
    const QString pass_key = QStringLiteral("password");

    explicit UserActionDialog(QWidget * parent = 0);
    ~UserActionDialog();


    void buildErrFields(const QString & err) {
        inputs << FormInput::createMessage(err);
    }

    void buildLoginFields(const QString & login_val, const QString & password_val, const QString & login_label, const QString & password_label) {
        inputs << FormInput::createStr(login_key, login_label, login_val);
        inputs << FormInput::createStr(pass_key, password_label, password_val);
    }

    void buildCaptchaFields(const QPixmap & captcha_img) {
        inputs << FormInput(captcha_img);
        inputs << FormInput::createStr(captcha_key, QStringLiteral("Captcha value"));
    }

    void buildLoginWithCaptchaForm(const QPixmap & captcha_img, const QString & err, const QString & login_val, const QString & password_val, const QString & login_label = QStringLiteral("Login"), const QString & password_label = QStringLiteral("Password"));
    void buildLoginForm(const QString & err = QString(), const QString & login_val = QString(), const QString & password_val = QString(), const QString & login_label = QStringLiteral("Login"), const QString & password_label = QStringLiteral("Password"));
    void buildCaptchaForm(const QPixmap & captcha_img);
    void buildToolbarButtonForm(const QString & name = QString(), const QString & path = QString());
    void buildToolbarForm(const QString & name = QString());
    void buildPresetForm(const QString & name = QString());
    void buildImportForm(const QString & text = QString());
    void buildForm(const QList<FormInput> & inputs, const QString & title = QStringLiteral("Some form"));
    void extendForm(const QList<FormInput> & inputs);

    QString getValue(const QString & name);

public slots:
    inline int exec() {
        if (!finalized) {
            insertButtons();
            adjustSize();
        }

        if (!Core::ThreadUtils::livesInCurrThread(this)) {
            QMetaObject::invokeMethod((QDialog *)(this), "exec", Qt::BlockingQueuedConnection);
            return result();
        }
        else return QDialog::exec();
    }

protected slots:
    inline void actionRequired() {
        FormInput input = actions.value(sender());
        QMetaObject::invokeMethod(input.obj, input.slot, Qt::AutoConnection, Q_ARG(QString &, input.value));
    }

    inline void browseClicked() {
        QString path = QFileDialog::getExistingDirectory(this, tr("Choose path"));
        QString key = sender() -> property("owner_key").toString();
        ((QLineEdit*)elements.value(key).second) -> setText(path);
    }


private:
    void insertElem(QGridLayout * l, QWidget * w) {
        l -> addWidget(w, l -> rowCount(), 0, 1, 3, Qt::AlignCenter);
    }

    void insertPair(QGridLayout * l, QWidget * w1, QWidget * w2) {
        int row = l -> rowCount();
        l -> addWidget(w1, row, 0);
        l -> addWidget(w2, row, 1, 1, 2);
    }

    void insertPairPlus(QGridLayout * l, QWidget * w1, QWidget * w2, QWidget * w3) {
        int row = l -> rowCount();
        l -> addWidget(w1, row, 0);
        l -> addWidget(w2, row, 1);
        l -> addWidget(w3, row, 2);
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

    QWidget * registerItem(FormInput & input);
    void createElement(FormInput input, QGridLayout * l);
    void createUrl(FormInput input, QGridLayout * l);
    void createImage(FormInput input, QGridLayout * l);
    void createAction(FormInput input, QGridLayout * l);
    void createMessage(FormInput input, QGridLayout * l);

    void proceedInputs(const QList<FormInput> & inputs);
    void insertButtons();

    Ui::UserActionDialog * ui;
    QHash<QString, QPair<FormInputType, QWidget *> > elements;
    QHash<QObject *, FormInput> actions;
    QList<FormInput> inputs;
    QWidget * layer;
    bool finalized;
};

#endif // USER_ACTION_DIALOG_H
