#ifndef SEARCH_CONFIGURATOR
#define SEARCH_CONFIGURATOR

#include <qlistwidget.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qcheckbox.h>
#include <qgroupbox.h>
#include <qradiobutton.h>

#include "modules/models/service/search_settings.h"
#include "modules/controls/accordion.h"

namespace Controls {
    class SearchConfigurator : public Accordion {
        Q_OBJECT

        void initUI();
        void initiateSources();

        QPushButton * activationBtn;

        QScrollArea * predicatesArea;
        QWidget * predicatesAreaBody;
        QListWidget * textPredicates;
        QPushButton * addStylePredicate;
        QPushButton * addPredicate;
        QComboBox * stylePredicate;
        QListWidget * stylePredicates;
        QLineEdit * textPredicate;
        QScrollArea * locationsArea;
        QWidget * locationsAreaBody;
        QListWidget * sitesList;
        QCheckBox * inSites;
        QListWidget * tabsList;
        QListWidget * driveList;
        QCheckBox * inComputer;
        QCheckBox * inTabs;
        QScrollArea * limitationsArea;
        QWidget * limitationsAreaBody;
        QGroupBox * searchInGroup;
        QCheckBox * byPopular;
        QCheckBox * byOwns;
        QCheckBox * byOrigins;
        QCheckBox * byForeign;
        QGroupBox * searchByGroup;
        QRadioButton * byTitle;
        QRadioButton * byTag;
        QRadioButton * byArtist;
        QRadioButton * bySong;

        bool has_not_connected;
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
