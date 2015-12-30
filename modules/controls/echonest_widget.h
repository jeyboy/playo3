#ifndef ECHONEST_WIDGET_H
#define ECHONEST_WIDGET_H

#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlistwidget.h>

#include <qformlayout.h>
#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qradiobutton.h>

#include "modules/core/web/apis/service/echonest_api.h"
#include "modules/core/web/utils/web_manager.h"
#include "accordion.h"

#include "echonest_parts/artist_info.h"

namespace Controls {
    class EchonestWidget : public Accordion {
        Q_OBJECT

        void artistInfoGeneration(QWidget * base);
        void songsSearch(QWidget * base);
        void basicPlaylistGeneration(QWidget * base);

        QStringList genresList();
        QStringList stylesList();
        QStringList moodsList();

        // basic playlist
        QList<QLineEdit *> basicPlaylistArtists;
        QList<QComboBox *> basicPlaylistGenres;

    //    QComboBox * createGenresCombo(QWidget * parent);
    //    QComboBox * createMoodsCombo(QWidget * parent);

        Echonest::ArtistInfo * artistInfo;
        QRadioButton * artistTypeCheck, * genreTypeCheck;

        Controls::Accordion * artistAccordion;
    public:
        explicit EchonestWidget(QWidget * parent = 0);
    //    ~EchonestWidget();

        void createSearchResultBar(const QStringList & predicates);

    protected:
        void clearData();

    private slots:
        void onArtistInfoButtonClicked();
        void onBasicPlaylistGenerateClicked();
    };
}

#endif // ECHONEST_DIALOG_H
