#include "icon_provider.h"

using namespace Core;

IconProvider::IconProvider() {}
IconProvider::~IconProvider() {}

QIcon IconProvider::fileIcon(const QString & filename, QString extension) {
    if (!obj().icons.contains(extension)) {
        QFileInfo fileInfo(filename);
        if (fileInfo.isFile()) {
            if (fileInfo.exists()) {
                QIcon res = obj().iconProvider.icon(fileInfo);
                obj().icons.insert(extension, res);
                return res;
            }
            else return QApplication::style() -> standardIcon(QStyle::SP_FileIcon);
        }
        else return QApplication::style() -> standardIcon(QStyle::SP_DriveNetIcon);
    }

    return obj().icons.value(extension);
}

QIcon IconProvider::dirIcon() {
    return obj().iconProvider.icon(QFileIconProvider::Folder);
//    QApplication::style() -> standardIcon(QStyle::SP_DirIcon);
}
