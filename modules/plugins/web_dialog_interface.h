#ifndef WEB_DIALOG_INTERFACE
#define WEB_DIALOG_INTERFACE

#include <qdialog.h>
#include <QNetworkAccessManager>

class WebDialogInterface {
public:
    virtual ~WebDialogInterface() {}
    virtual QDialog * createDialog(QWidget * parent, QNetworkAccessManager * manager, QUrl url, QString title);
    virtual void registerActions(QObject * api);
    virtual void closeDialog();
};

#define WebDialogInterface_iid "org.qt-project.lalka.WebDialogInterface"

Q_DECLARE_INTERFACE(WebDialogInterface, WebDialogInterface_iid)

#endif //WEB_DIALOG_INTERFACE
