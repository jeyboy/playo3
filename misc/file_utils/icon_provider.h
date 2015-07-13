#ifndef ICON_PROVIDER_H
#define ICON_PROVIDER_H

#include <qicon.h>
#include <qhash.h>
#include <qpixmapcache.h>
#include <qfileiconprovider.h>
#include <qstyle.h>
#include <qapplication.h>

namespace Playo3 {
    class IconProvider {
    public:
        inline ~IconProvider() {}

        static IconProvider * instance();
        static QIcon fileIcon(const QString filename, QString extension);
        static QIcon dirIcon();
        static QIcon remoteIcon();
        static QIcon missedIcon();
        inline static void close() { delete self; }
    private:
        inline IconProvider() {}

        static IconProvider * self;

        QHash<QString, QIcon> icons;
        QFileIconProvider iconProvider;
    };
}

#endif // ICON_PROVIDER_H
