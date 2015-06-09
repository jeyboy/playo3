#include "web_dialog.h"
#include "ui_web_dialog.h"

WebDialog::WebDialog(QWidget * parent, QNetworkAccessManager * manager, QUrl & url, QString title) :
    QDialog(parent), ui(new Ui::WebDialog) {

    ui -> setupUi(this);

    setWindowTitle(title);

    ui -> webView -> page() -> setNetworkAccessManager(manager);

    connect(ui -> webView, SIGNAL(urlChanged(const QUrl &)), SLOT(onUrlChanged(const QUrl &)));

    ui -> webView -> load(QUrl(url));
    ui -> webView -> show();
}

WebDialog::~WebDialog() {
    delete ui;
}

void WebDialog::setApiResponse(QString response) {
    if (response == "accept")
        accept();
    else if (response == "reject")
        reject();
    else if (response.length() > 0)
        ui -> webView -> load(QUrl(response));
}

////////////////////////////////////////////////////////////
/// SLOTS
////////////////////////////////////////////////////////////

void WebDialog::onUrlChanged(const QUrl & url) {
    emit urlChanged(url);
}
