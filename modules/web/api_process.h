#ifndef API_PROCESS_H
#define API_PROCESS_H

#include <QObject>
#include <QUrl>
#include <QtConcurrent/QtConcurrent>
#include <QFutureWatcher>
#include <QApplication>

#include "func_container.h"
#include "dialogs/captchadialog.h"

//template <class T>
struct ApiFuncContainer {
    ApiFuncContainer() { }
    ApiFuncContainer(FuncContainer & container, QString & uid) {
        func = container;
        this -> uid = uid;
    }
    ~ApiFuncContainer() {}

    QString uid;
    FuncContainer func;
    QJsonObject result;
};


class ApiProcess : public QObject {
    Q_OBJECT   
public:
    ~ApiProcess() {
        qDeleteAll(processes);
        processes.clear();

        delete captchaDialog;
    }

    static ApiProcess * instance();
    static void close() {
        delete self;
    }

    void start(QFuture<ApiFuncContainer *> future);
    CaptchaDialog * getCaptchaDialog();

signals:
    void routineFinished(QJsonObject &);

protected slots:
//    void downloadConnectionResponsed();
//    void onTimer();
    void finished();
    void showCaptcha();

//signals:
//    void slotChanged(QString message);
//    void downloadProgress(ModelItem * item, int percentDone);
//    void downloadFinished(ModelItem * item, bool success);
//    void downloadError(ModelItem * item, QString message);

private:
    ApiProcess() {
        captchaDialog = new CaptchaDialog(QApplication::allWidgets().first());
    }

    QList<QFutureWatcher<ApiFuncContainer *> *> processes;
    static ApiProcess *self;
    CaptchaDialog * captchaDialog;
};

#endif // API_PROCESS_H
