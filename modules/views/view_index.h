#ifndef VIEWS_INDEX_H
#define VIEWS_INDEX_H

#include "modules/core/playlist_types.h"

#include "service/download_view.h"
#include "service/search_view.h"

#include "list_view.h"
#include "tree_view.h"
#include "level_tree_view.h"

#include "web/vk_view.h"
#include "web/vk_rel_view.h"
#include "web/soundcloud_view.h"
#include "web/od_view.h"

namespace Views {
    class ViewFactory {
    public:
        static IView * build(QWidget * parent, const Params & settings, QJsonObject * attrs) {
            switch(settings.type) {
                case playlist_level: return new ListView(parent, settings, attrs);
                case playlist_level_tree: return new LevelTreeView(parent, settings, attrs);
                case playlist_tree: return new TreeView(parent, settings, attrs);
                case playlist_vk: return new VkView(parent, settings, attrs);
                case playlist_vk_rel: return new VkRelView(parent, settings, attrs);
                case playlist_soundcloud: return new SoundcloudView(parent, settings, attrs);
                case playlist_search: return new SearchView(parent, settings, attrs);
                case playlist_od: return new OdView(parent, settings, attrs);

                default: return 0;
            }
        }
    };
}

#endif // VIEWS_INDEX_H
