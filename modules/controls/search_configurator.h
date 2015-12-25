#ifndef SEARCH_CONFIGURATOR
#define SEARCH_CONFIGURATOR

#include <qlistwidget.h>

#include "modules/models/service/search_settings.h"
#include "modules/controls/accordion.h"

namespace Controls {
    class SearchConfigurator : Accordion {
        QScrollArea * locationsArea();
        QScrollArea * predicatesArea();
        QScrollArea * limitationsArea();
        void initiateSources();

        QPushButton * activationBtn;
    public:
        SearchConfigurator(QWidget * parent, QPushButton * activationBtn = 0);
        SearchSettings params();
    protected slots:
        void on_addPredicate_clicked();
        void on_textPredicates_itemActivated(QListWidgetItem * item);
        void on_stylePredicates_itemActivated(QListWidgetItem * item);
        void on_addStylePredicate_clicked();
        void on_driveList_itemClicked(QListWidgetItem * item);
        void on_inComputer_toggled(bool checked);
        void on_inTabs_toggled(bool checked);
        void on_tabsList_itemClicked(QListWidgetItem *item);
        void on_inSites_toggled(bool checked);
        void on_sitesList_itemClicked(QListWidgetItem *item);
        void on_accordeon_currentChanged(int index);
    };
}

#endif // SEARCH_CONFIGURATOR
