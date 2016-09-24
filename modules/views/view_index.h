#ifndef VIEWS_INDEX_H
#define VIEWS_INDEX_H

#include "service/download_view.h"
#include "service/search_view.h"

#include "list_view.h"
#include "tree_view.h"
#include "level_tree_view.h"
#include "web/web_view.h"

namespace Views {
    class ViewFactory {
    public:
        static IView * build(QWidget * parent, const Params & settings, QJsonObject * attrs) {
            switch(settings.data_type) {
                case dt_level:          return new ListView(parent, settings, attrs);
                case dt_level_tree:     return new LevelTreeView(parent, settings, attrs);
                case dt_tree:           return new TreeView(parent, settings, attrs);
                case dt_search:         return new SearchView(parent, settings, attrs);

                default: {
                    if (DST_IS_WEB(settings.data_type))
                        return new WebView(parent, settings, attrs);
                    else
                        return new ListView(parent, settings, attrs); //0;
                }
            }
        }
    };
}

#endif // VIEWS_INDEX_H
