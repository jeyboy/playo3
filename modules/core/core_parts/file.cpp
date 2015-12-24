#include "file.h"

using namespace Core;

File::File(QString fileName, Playlist * parent, int pos) : IItem(parent, DEFAULT_TITLE, pos) {
    QString ext;
    if (FilenameConversions::extractExtension(fileName, ext))
        setExtension(ext);

    setTitle(fileName);
}
File::File(QString filePath, QString fileName, Playlist * parent, int pos)
    : IItem(parent, DEFAULT_TITLE, pos) {

    QString ext;
    if (FilenameConversions::extractExtension(fileName, ext))
        setExtension(ext);

    setTitle(fileName);
    setPath(filePath);
}
