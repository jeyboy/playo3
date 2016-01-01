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
#include "echonest_parts/basic_playlist.h"
#include "echonest_parts/song_search.h"

namespace Controls {
    class EchonestWidget : public Accordion {
        Q_OBJECT

        QStringList genresList();
        QStringList stylesList();
        QStringList moodsList();

        QStringList genres;
        QStringList styles;
        QStringList moods;
    public:
        explicit EchonestWidget(QWidget * parent = 0);
    private slots:
        void moveInProcess();
        void playlistGenerationNeed(const QString & title, QStringList & predicates);
    };
}

#endif // ECHONEST_DIALOG_H
