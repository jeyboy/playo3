#ifndef ECHONEST_DIALOG_H
#define ECHONEST_DIALOG_H

#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qgroupbox.h>

#include <qformlayout.h>
#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qradiobutton.h>


#include "base_dialog.h"
#include "modules/core/web/apis/service/echonest_api.h"
#include "modules/core/web/utils/web_manager.h"
#include "modules/controls/accordion.h"
#include "modules/views/service/search_view.h"

namespace Ui { class EchonestDialog; }

class EchonestDialog : public BaseDialog {
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
    QStringList stylesList();
    QStringList moodsList();

    // basic playlist
    QList<QLineEdit *> basicPlaylistArtists;
    QList<QComboBox *> basicPlaylistGenres;

//    QComboBox * createGenresCombo(QWidget * parent);
//    QComboBox * createMoodsCombo(QWidget * parent);

    QLineEdit * artistName;
    QRadioButton * artistTypeCheck, * genreTypeCheck;

    Ui::EchonestDialog * ui;
    Controls::Accordion * artistAccordion;
};

#endif // ECHONEST_DIALOG_H
