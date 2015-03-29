#include "api_process.h"

ApiProcess *ApiProcess::self = 0;

ApiProcess *ApiProcess::instance() {
    if(!self)
        self = new ApiProcess();
    return self;
}

void ApiProcess::finished() {
    QFutureWatcher<ApiFuncContainer *> * initiator = (QFutureWatcher<ApiFuncContainer *> *) sender();
    ApiFuncContainer * func = initiator -> result();
    connect(this, SIGNAL(routineFinished(QJsonObject &)), func -> func.obj, func -> func.slot);
    emit routineFinished(func -> result);
    disconnect(this, SIGNAL(routineFinished(QJsonObject &)), func -> func.obj, func -> func.slot);
    processes.removeOne(initiator);
}

void ApiProcess::showCaptcha() {
    captchaDialog -> clearText();
    captchaDialog -> exec();
}

void ApiProcess::start(QFuture<ApiFuncContainer *> future) {
    QFutureWatcher<ApiFuncContainer *> * initiator = new QFutureWatcher<ApiFuncContainer *>();
    processes.append(initiator);
    connect(initiator, SIGNAL(finished()), this, SLOT(finished()));
    initiator -> setFuture(future);
}

CaptchaDialog * ApiProcess::getCaptchaDialog() {
    return captchaDialog;
}
