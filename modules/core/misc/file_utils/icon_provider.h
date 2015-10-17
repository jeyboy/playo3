#ifndef ICON_PROVIDER_H
#define ICON_PROVIDER_H

#include <qicon.h>
#include <qhash.h>
#include <qpixmapcache.h>
#include <qfileiconprovider.h>
#include <qstyle.h>
#include <qapplication.h>

#include "modules/core/interfaces/singleton.h"

namespace Core {
    class IconProvider : public Singleton<IconProvider> {
        IconProvider(); friend class Singleton<IconProvider>;

        QHash<QString, QIcon> icons;
        QFileIconProvider iconProvider;
    public:
        ~IconProvider();

        static QIcon fileIcon(const QString & filename, QString extension);
        static QIcon dirIcon();
    };
}

#endif // ICON_PROVIDER_H
