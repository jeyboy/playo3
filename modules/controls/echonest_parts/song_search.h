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

namespace Controls {
    namespace Echonest {
        class SongSearch : public QWidget {
            Q_OBJECT

            void generateLayout();

            QStringList genresList;

            bool combined;
            QLineEdit * artist;
            QLineEdit * title;

            QList<QLineEdit *> descriptions;
            QList<QDoubleSpinBox *> descriptionPowers;
            QList<QComboBox *> styles;
            QList<QDoubleSpinBox *> stylePowers;
            QList<QComboBox *> moods;
            QList<QDoubleSpinBox *> moodPowers;

            float song_min_hotttnesss;
            float song_max_hotttnesss;

            float min_danceability;
            float max_danceability;

            float min_energy;
            float max_energy;

            float min_liveness;
            float max_liveness;

            float min_speechiness;
            float max_speechiness;

            float min_acousticness;
            float max_acousticness;

            float min_tempo;
            float max_tempo;

            float artist_min_familiarity;
            float artist_max_familiarity;

            int mode; // (minor, major) 0, 1

            QRadioButton * artistTypeCheck, * genreTypeCheck;
        public:
            explicit SongSearch(const QStringList & stylesList, const QStringList & moodsList, QWidget * parent = 0);

        signals:
            void moveInProcess();
            void playlistGenerationNeed(const QString & title, QStringList & predicates);
        private slots:
            void onSearchClicked();
        };
    }
}

#endif // SONG_SEARCH_H
