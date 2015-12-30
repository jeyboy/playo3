#ifndef BASIC_PLAYLIST_H
#define BASIC_PLAYLIST_H

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


namespace Controls {
    namespace Echonest {
        class BasicPlaylist : public QWidget {
            Q_OBJECT

            void generateLayout();

            QStringList genresList;

            QList<QLineEdit *> basicPlaylistArtists;
            QList<QComboBox *> basicPlaylistGenres;

            QRadioButton * artistTypeCheck, * genreTypeCheck;
        public:
            explicit BasicPlaylist(const QStringList & genresList, QWidget * parent = 0);

        signals:
            void playlistGenerationNeed(QString & title, QStringList & predicates);
        private slots:
            void onBasicPlaylistGenerateClicked();
        };
    }
}

#endif // BASIC_PLAYLIST_H
