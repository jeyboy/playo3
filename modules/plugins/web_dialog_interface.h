#ifndef WEB_DIALOG_INTERFACE
#define WEB_DIALOG_INTERFACE

#include <qdialog.h>
#include <QNetworkAccessManager>

class WebDialogInterface {
public:
    virtual QDialog * createDialog(QWidget * parent, QNetworkAccessManager * manager, QUrl url, QString title) = 0;
    virtual void registerActions(QObject * api) = 0;
    virtual void closeDialog() = 0;
};

#define WebDialogInterface_iid "org.qt-project.lalka.WebDialogInterface"

Q_DECLARE_INTERFACE(WebDialogInterface, WebDialogInterface_iid)

#endif //WEB_DIALOG_INTERFACE
