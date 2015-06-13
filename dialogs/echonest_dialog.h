#ifndef ECHONEST_DIALOG_H
#define ECHONEST_DIALOG_H

#include <qdialog.h>
#include "modules/web/service_api/echonest_api.h"

namespace Ui {
    class EchonestDialog;
}

class EchonestDialog : public QDialog {
    Q_OBJECT
public:
    explicit EchonestDialog(QWidget * parent = 0);
    ~EchonestDialog();

private slots:
    void on_findArtistSongs_clicked();

private:
    Ui::EchonestDialog * ui;
};

#endif // ECHONEST_DIALOG_H
