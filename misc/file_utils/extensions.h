#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <QDir>
#include "data_store.h"
#include "dialogs/extension_dialog.h"

namespace Playo3 {
    class Extensions {
    public:
        static QString folderName(QFileInfo & info);

        ~Extensions() {
            ext -> clear();
            ext -> write("active", activeFilterName());

            QJsonObject filtersObj;
            foreach(QString key, filters.keys()) {
                filtersObj.insert(key, QJsonValue::fromVariant(filters.value(key)));
            }

            ext -> write("filters", filtersObj);
            ext -> save();
            delete ext;
        }

        static Extensions * instance();
        static void close() {
            delete self;
        }

        bool restoreExtension(QString & file_path);
        QFileInfoList folderFiles(QFileInfo file);
        QFileInfoList folderDirectories(QFileInfo file);

        bool respondToExtension(QString ext) const;
        QStringList activeFilterList() const;
        QStringList filterList(const QString & name) const;
        void filterListUpdate(const QString & name, QStringList & list);
        void addNewPreset(QString name);
        void removePreset(QString name);

        inline void setActiveFilterName(const QString & name) { activeFilter = name; }
        inline QString activeFilterName() const { return activeFilter; }
        inline QStringList presetsList() const { return QStringList(filters.keys()); }

    private:
        Extensions();
        void initSignatures();

        static Extensions * self;

        QString activeFilter;
        QHash<QString, QStringList> filters;
        QHash<QString, QString> ext_signatures;
        DataStore * ext;
    };
}

#endif // EXTENSIONS_H
