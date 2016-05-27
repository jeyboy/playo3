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

        QWidget * predicatesAreaBody;

        QComboBox * stylePredicate;

        QLineEdit * textPredicate;

        QPushButton * activationBtn;
        QPushButton * addStylePredicate;
        QPushButton * addPredicate;

        QListWidget * textPredicates;
        QListWidget * stylePredicates;
        QListWidget * sitesList;
        QListWidget * tabsList;
        QListWidget * driveList;

        QCheckBox * inSites;
        QCheckBox * inComputer;
        QCheckBox * inTabs;

        QRadioButton * byTitle;
        QRadioButton * byTag;
        QRadioButton * byArtist;
        QRadioButton * bySongName;
        QRadioButton * bySet;
        QRadioButton * byLyric;
        QRadioButton * byAbc;

        QCheckBox * byPopular;
        QCheckBox * byOwns;
        QCheckBox * byOrigins;
        QCheckBox * byMixes;
        QCheckBox * byForeign;
        QCheckBox * byCyrillic;
        QCheckBox * byRelativity;

        bool has_not_connected;

        void initiateSources();
        QWidget * initLocations();
        QWidget * initPredicates();
        QWidget * initLimitations();
    public:
        enum SearchSettingsBlocks {
            none = 0,
            block_computer = 1,
            block_tabs = 2,
            block_sites = 4,
            block_all = block_computer | block_tabs | block_sites,
            block_tabs_and_sites = block_tabs | block_sites
        };

        static SearchSettings buildParams(int limitPerPredicate = 999999, const SearchSettingsBlocks & blocks = block_all, const QStringList & predicates = QStringList(), const QStringList & ganres = QStringList());

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
        void on_tabsList_itemClicked(QListWidgetItem * item);
        void on_inSites_toggled(bool checked);
        void on_sitesList_itemClicked(QListWidgetItem * item);
        void onFocusChanged(QWidget * old, QWidget * now);
    };
}

#endif // SEARCH_CONFIGURATOR
