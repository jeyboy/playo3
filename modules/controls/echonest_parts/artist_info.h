#ifndef ARTIST_INFO_H
#define ARTIST_INFO_H

#include <qlabel.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qgroupbox.h>
#include <qlistwidget.h>

#include <qformlayout.h>
#include <qboxlayout.h>
#include <qcheckbox.h>
#include <qradiobutton.h>

#include "../accordion.h"
#include "modules/core/web/apis/service/echonest_api.h"
#include "modules/core/web/utils/web_manager.h"

namespace Controls {
    namespace Echonest {
        class ArtistInfo : public QWidget {
            Q_OBJECT

            QLineEdit * artistName;

            Controls::Accordion * artistAccordion;

            generateLayout();
        public:
            explicit ArtistInfo(QWidget * parent = 0);
//        signals:
//            void submited();
        protected slots:
            void onArtistInfoButtonClicked();
        };
    }
}

#endif // ARTIST_INFO_H
