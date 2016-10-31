#ifndef ITEM_TOOLBAR_FEATURES_H
#define ITEM_TOOLBAR_FEATURES_H

#include <qtoolbar.h>
#include "dockbars.h"
#include "modules/core/web/web_apis.h"
#include "modules/core/core_parts/item.h"

#define ICO_PREFIX LSTR(":/item_tools/item_")
#define PERM_ERROR(name) "Permissions required" << name << "required on some additional permissions or this service not respondable to this action";
#define ADD_FEATURE_ITEM(postfix, ico, desc, slot, check_func) \
    features.insert((quint64)(void *)(&slot), ItemToolbarFeature(toolbar, ICO_PREFIX + ico + postfix, desc, this, SLOT(slot()), &check_func))

namespace Presentation {
    struct ItemToolbarFeature {
        ItemToolbarFeature(
            QToolBar * toolbar = 0, const QString & ico = QString(), const QString & desc = QString(), QObject * obj = 0 , const char * btn_slot = 0, bool (ISource::*check_func)() = 0
        ) : btn(0), ico(ico), desc(desc), check_func(check_func) {

            int slot_len = 0;
            if (btn_slot != 0) {
                slot_len = strlen(btn_slot);
                strncpy(slot, btn_slot, slot_len);
            }

            slot[slot_len] = '\0';

            if (toolbar && check_func)
                btn = toolbar -> addAction(QIcon(ico), desc, obj, btn_slot);
        }

        QAction * btn;
        QString ico;
        QString desc;
        bool (ISource::*check_func)();
        char slot[512];
    };

    class ItemToolbarFeatures : public QObject {
        Q_OBJECT
    protected:
        QToolBar * toolbar;

        QAction * more_items_btn;
        QAction * refresh_tab_btn;
        QHash<quint64, ItemToolbarFeature> features;

        virtual Core::IItem * targetItem() = 0;
        virtual Views::IView * targetView() = 0;
        virtual QString sentence() = 0;

        void prepareToolbar(const QString & postfix = QString());
    public:
        ItemToolbarFeatures(QToolBar * toolbar);

        QToolBar * targetToolbar() { return toolbar; }
    public slots:
        void hide() { toolbar ->  hide(); }
        void updateToolbar();
        void appendToMenu(QMenu * menu);

        void loadMoreItem();
        void refreshTab();

        void openAudioforItem();
        void openAudioAlbumforItem();
        void openAudioPlaylistforItem();

        void openVideoforItem();
        void openVideoAlbumforItem();
        void openVideoPlaylistforItem();

        void openRecsforItemArtist();
        void openSimillarAudiobyArtist();
        void openAudioforArtist();

        void openAudioAlbumsForArtist();
        void openPlaylistsForArtist();
        void openVideoforArtist();
        void openVideoAlbumforArtist();
        void openVideoPlaylistforArtist();

        void openRecsforUser();
        void openArtistsforUser();
        void openAudioforUser();
        void openAudioAlbumsforUser();
        void openAudioPlaylistsforUser();

        void openVideoforUser();
        void openVideoAlbumsforUser();
        void openVideoPlaylistsforUser();

        void openRecForTag();
        void openArtistForTag();
        void openAudioForTag();
        void openAudioAlbumForTag();
        void openAudioPlaylistForTag();

        void openVideoForTag();
        void openVideoAlbumForTag();
        void openVideoPlaylistForTag();

        void openRecommendationsForLabel();
        void openArtistsForLabel();

        void openAudioForLabel();
        void openAudioAlbumForLabel();
        void openAudioPlaylistForLabel();

        void openVideoForLabel();
        void openVideoAlbumForLabel();
        void openVideoPlaylistForLabel();
    };

    class PlayedItemToolbarFeatures : public ItemToolbarFeatures {
    protected:
        Core::IItem * targetItem() { return DataFactory::obj().playedItem(); }
        Views::IView * targetView() { return Dockbars::obj().playedView(); }
        QString sentence() { return LSTR("played item"); }
    public:
        PlayedItemToolbarFeatures(QToolBar * toolbar) : ItemToolbarFeatures(toolbar) { prepareToolbar(LSTR("_pl")); }
    };

    class SelectedItemToolbarFeatures : public ItemToolbarFeatures {
    protected:
        Core::IItem * targetItem() {
            IView * view = targetView();
            return view ? view -> selectedItem() : 0;
        }
        Views::IView * targetView() { return Dockbars::obj().activeView(); }
        QString sentence() { return LSTR("selected item"); }
    public:
        SelectedItemToolbarFeatures(QToolBar * toolbar) : ItemToolbarFeatures(toolbar) { prepareToolbar(); }
    };
}

#endif // ITEM_TOOLBAR_FEATURES_H
