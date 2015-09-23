#include "icon_provider.h"

using namespace Playo3;

IconProvider * IconProvider::self = 0;

IconProvider * IconProvider::instance() {
    if(!self)
        self = new IconProvider();
    return self;
}

QIcon IconProvider::fileIcon(const QString & filename, QString extension) {
    if (!instance() -> icons.contains(extension)) {
        QFileInfo fileInfo(filename);
        if (fileInfo.isFile()) {
            if (fileInfo.exists()) {
                QIcon res = instance() -> iconProvider.icon(fileInfo);
                instance() -> icons.insert(extension, res);
                return res;
            }
            else return QApplication::style() -> standardIcon(QStyle::SP_FileIcon);
        }
        else return QApplication::style() -> standardIcon(QStyle::SP_DriveNetIcon);
    }

    return instance() -> icons.value(extension);
}

QIcon IconProvider::dirIcon() {
    return instance() -> iconProvider.icon(QFileIconProvider::Folder);
//    QApplication::style() -> standardIcon(QStyle::SP_DirIcon);
}
