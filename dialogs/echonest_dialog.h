#ifndef ECHONEST_DIALOG_H
#define ECHONEST_DIALOG_H

#include <qdialog.h>
#include <qlabel.h>
#include <QVBoxLayout>
#include "modules/web/service_api/echonest_api.h"
#include "misc/web_utils/custom_network_access_manager.h"
#include "modules/controls/accordion.h"

namespace Ui {
    class EchonestDialog;
}

class EchonestDialog : public QDialog {
    Q_OBJECT
public:
    explicit EchonestDialog(QWidget * parent = 0);
    ~EchonestDialog();

    void on_findArtistSongs_clicked();
private slots:
//    void on_findArtistSongs_clicked();

private:
    Ui::EchonestDialog * ui;
    Accordion * accordion;
};

#endif // ECHONEST_DIALOG_H
