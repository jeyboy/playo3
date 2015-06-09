#ifndef WEB_DIALOG_H
#define WEB_DIALOG_H

#include <QDialog>
#include <QtWebKitWidgets>

namespace Ui {
    class WebDialog;
}

class WebDialog : public QDialog {
    Q_OBJECT

public:
    explicit WebDialog(QWidget * parent, QNetworkAccessManager * manager, QUrl & url, QString title);
    ~WebDialog();
signals:
    void urlChanged(const QUrl &);
public slots:
    void setApiResponse(QString);
private slots:
    void onUrlChanged(const QUrl &);

private:
    Ui::WebDialog * ui;
};

#endif // WEB_DIALOG_H
