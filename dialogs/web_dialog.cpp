#include "web_dialog.h"
#include "ui_web_dialog.h"
#include <qdebug.h>

WebDialog::WebDialog(QWidget * parent, WebApi * apiClass, QString title) :
    QDialog(parent), ui(new Ui::WebDialog), api(apiClass) {

    ui -> setupUi(this);

    setWindowTitle(title);

    ui -> webView -> page() -> setNetworkAccessManager(Web::manager());

    connect(ui -> webView, SIGNAL(urlChanged(const QUrl &)), SLOT(urlChanged(const QUrl &)));

    ui -> webView -> load(QUrl(api -> authUrl()));
    ui -> webView -> show();
}

WebDialog::~WebDialog() {
    delete ui;
}

////////////////////////////////////////////////////////////
/// SLOTS
////////////////////////////////////////////////////////////

void WebDialog::urlChanged(const QUrl & url) {
    QString res = api -> proceedAuthResponse(url);
    qDebug() << "Hula: " << url << " | " << res;
    if (res == "accept")
        accept();
    else if (res == "reject")
        reject();
    else if (res.length() > 0)
        ui -> webView -> load(QUrl(res));
}
