#ifndef FILE_ITEM_H
#define FILE_ITEM_H

//#include <QFile>
//#include <QJsonObject>

#include "item_interface.h"

namespace Playo3 {
    class FileItem : public ItemInterface {
    public:
        FileItem(QJsonObject * hash, ModelItem * parent = 0);
        FileItem(const QString filepath, ModelItem * parent = 0);
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
