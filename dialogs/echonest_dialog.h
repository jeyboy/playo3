#ifndef ECHONEST_DIALOG_H
#define ECHONEST_DIALOG_H

#include <qdialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>

#include <qformlayout.h>
#include <qboxlayout.h>

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

private slots:
    void onArtistInfoButtonClicked();

private:
    QComboBox * createGenresCombo(QWidget * parent);
    QComboBox * createMoodsCombo(QWidget * parent);

    QLineEdit * artistName;

    Ui::EchonestDialog * ui;
    Accordion * artistAccordion;
};

#endif // ECHONEST_DIALOG_H
