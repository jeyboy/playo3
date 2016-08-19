#ifndef VIEWS_INDEX_H
#define VIEWS_INDEX_H

#include "service/download_view.h"
#include "service/search_view.h"

#include "list_view.h"
#include "tree_view.h"
#include "level_tree_view.h"

#include "web/vk_view.h"
#include "web/soundcloud_view.h"
#include "web/od_view.h"
#include "web/fourshared_view.h"

namespace Views {
    class ViewFactory {
    public:
        static IView * build(QWidget * parent, const Params & settings, QJsonObject * attrs) {
            switch(settings.type) {
                case dt_level: return new ListView(parent, settings, attrs);
                case dt_level_tree: return new LevelTreeView(parent, settings, attrs);
                case dt_tree: return new TreeView(parent, settings, attrs);
                case dt_web_vk: return new VkView(parent, settings, attrs);
                case dt_web_sc: return new SoundcloudView(parent, settings, attrs);
                case dt_web_od: return new OdView(parent, settings, attrs);
                case dt_web_fourshared: return new FoursharedView(parent, settings, attrs);
                case dt_search: return new SearchView(parent, settings, attrs);

                default: return new ListView(parent, settings, attrs); //0;
            }
        }
    };
}

#endif // VIEWS_INDEX_H
