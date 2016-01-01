#ifndef SONG_SEARCH_H
#define SONG_SEARCH_H

#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlistwidget.h>

#include <qformlayout.h>
#include <qboxlayout.h>
#include <qspinbox.h>

#include "modules/core/web/apis/service/echonest_api.h"
#include "modules/core/web/utils/web_manager.h"
#include "modules/controls/qxtspanslider.h"

#define SLIDER_MULTIPLIER .001

namespace Controls {
    namespace Echonest {
        class SongSearch : public QWidget {
            Q_OBJECT

            void generateLayout();

            QVBoxLayout * descsLayout;
            QVBoxLayout * stylesLayout;
            QVBoxLayout * moodsLayout;

            QStringList stylesList;
            QStringList moodsList;

            QCheckBox * combined;
            QLineEdit * artist;
            QLineEdit * title;

            QHash<QLineEdit *, QDoubleSpinBox *> descriptions;
            QHash<QComboBox *, QDoubleSpinBox *> styles;
            QHash<QComboBox *, QDoubleSpinBox *> moods;

            QxtSpanSlider * artist_familiarity;
            QxtSpanSlider * song_hotttnesss;
            QxtSpanSlider * song_danceability;
            QxtSpanSlider * song_energy;
            QxtSpanSlider * song_liveness;
            QxtSpanSlider * song_speechiness;
            QxtSpanSlider * song_acousticness;
            QxtSpanSlider * song_tempo;

            QComboBox * mode; // (minor, major) 0, 1

//            QRadioButton * artistTypeCheck, * genreTypeCheck;
        public:
            explicit SongSearch(const QStringList & stylesList, const QStringList & moodsList, QWidget * parent = 0);

        signals:
            void moveInProcess();
            void playlistGenerationNeed(const QString & title, QStringList & predicates);
        private slots:
            void onDescriptionAdd();
            void onDescriptionRemove();

            void onStyleAdd();
            void onStyleRemove();

            void onMoodAdd();
            void onMoodRemove();

            void onSearchClicked();
        };
    }
}

#endif // SONG_SEARCH_H
