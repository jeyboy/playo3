#include "file.h"
#include "modules/core/misc/file_utils/extensions.h"

using namespace Core;

File::File(QString fileName, Playlist * parent, int pos) : IItem(parent, DEFAULT_TITLE, pos) {
    setPath(fileName);
    QString ext;
    if (Extensions::obj().extractExtension(fileName, ext))
        setExtension(ext);

    setTitle(fileName);
}
File::File(QString filePath, QString fileName, Playlist * parent, int pos)
    : IItem(parent, DEFAULT_TITLE, pos) {

    QString ext;
    if (Extensions::obj().extractExtension(fileName, ext))
        setExtension(ext);

    setTitle(fileName);
    setPath(filePath);
}
