#ifndef ICON_PROVIDER_H
#define ICON_PROVIDER_H

#include <QIcon>
#include <QHash>
#include <QPixmapCache>
#include <QFileIconProvider>
#include <QStyle>
#include <QApplication>

namespace Playo3 {
    class IconProvider {
    public:
        ~IconProvider() {
        }

        static IconProvider * instance();
        static QIcon fileIcon(const QString filename, QString extension);
        static QIcon dirIcon();
        static QIcon remoteIcon();
        static QIcon missedIcon();
        static void close() {
            delete self;
        }

    private:
        IconProvider() {
        }

        static IconProvider * self;

        QHash<QString, QIcon> icons;
        QFileIconProvider iconProvider;
    };
}

#endif // ICON_PROVIDER_H
