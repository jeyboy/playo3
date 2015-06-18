#ifndef ECHONEST_DIALOG_H
#define ECHONEST_DIALOG_H

#include <qdialog.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qgroupbox.h>

#include <qformlayout.h>
#include <qboxlayout.h>
#include <qcheckbox.h>

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
    void onBasicPlaylistGenerateClicked();

private:
    void artistInfoGeneration(QWidget * base);
    void basicPlaylistGeneration(QWidget * base);

    QStringList genresList();
    QStringList moodsList();

    // basic playlist
    QCheckBox * artistTypeCheck, * genreTypeCheck;
    QList<QLineEdit *> basicPlaylistArtists;
    QList<QComboBox *> basicPlaylistGenres;

//    QComboBox * createGenresCombo(QWidget * parent);
//    QComboBox * createMoodsCombo(QWidget * parent);

    QLineEdit * artistName;

    Ui::EchonestDialog * ui;
    Accordion * artistAccordion;
};

#endif // ECHONEST_DIALOG_H
