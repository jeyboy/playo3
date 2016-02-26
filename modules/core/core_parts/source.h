#ifndef SOURCE
#define SOURCE

#include "item_interface.h"

namespace Core {
    class Source : public IItem {
        IItem * _parent_item;
    public:
        Source(IItem * parent, Web::SubType subType, QString & url, const QString & refresh_token = QString()) : IItem(0), _parent_item(parent) {
            setSubtype(subType);
            setPath(url);
            setRefreshPath(refresh_token);
        }
    };
}

#endif // SOURCE
