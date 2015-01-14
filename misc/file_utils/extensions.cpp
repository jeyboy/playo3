#include "extensions.h"

Extensions *Extensions::self = 0;

Extensions *Extensions::instance() {
    if(!self)
        self = new Extensions();
    return self;
}

QFileInfoList Extensions::folderFiles(QFileInfo file) {
    return QDir(file.filePath()).entryInfoList(activeFilterList(), QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
}

QFileInfoList Extensions::folderDirectories(QFileInfo file) {
    return QDir(file.filePath()).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
//    return QDir(file.filePath()).entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
}

bool Extensions::respondToExtension(QString ext) const {
    return activeFilterList().contains("*." + ext) || activeFilterList().contains("*.*");
}

QStringList Extensions::activeFilterList() const {
    if (filters.isEmpty())
        return QStringList();
    else
        return filters.value(activeFilter, filters.values().first());
}

QStringList Extensions::filterList(const QString & name) const {
    return filters.value(name, filters.values().first());
}

void Extensions::filterListUpdate(const QString & name, QStringList & newList) {
    filters.insert(name, newList);
}

void Extensions::addNewPreset(QString name) {
    filters.insert(name, QStringList());
}

void Extensions::removePreset(QString name) {
    filters.remove(name);
}
