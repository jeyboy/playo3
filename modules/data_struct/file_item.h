#ifndef FILE_ITEM_H
#define FILE_ITEM_H

//#include <QJsonObject>

#include <QFile>
#include "item_interface.h"

namespace Playo3 {
    class FileItem : public ItemInterface {
    public:
        FileItem(QJsonObject * hash, ItemInterface * parent = 0);
        FileItem(const QString filepath, ItemInterface * parent = 0);
        ~FileItem();

        bool removePhysicalObject();

        bool isExist() const;

        QJsonObject toJSON();
    protected:
        QString fileExtension(QString filePath);
        QString fileTitle(QString filePath);
    };
}

#endif // FILE_ITEM_H
