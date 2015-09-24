#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <qdir.h>
#include <qstringbuilder.h>

#include "data_store.h"
#include "dialogs/extension_dialog.h"

#define JSON_KEY QStringLiteral("filters")
#define JSON_ACTIVE_KEY QStringLiteral("active")
#define DEFAULT_ACTIVE QStringLiteral("all")

namespace Core {
    class Extensions {
    public:
        static QString folderName(QFileInfo & info);

        ~Extensions();

        static Extensions * instance();
        inline static void close() { delete self; }

        bool restoreExtension(QString & file_path, QString & restoredExt);
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
        void initMusicExtensions();

        static Extensions * self;

        QString activeFilter;
        QHash<QString, QStringList> filters;
        QHash<QString, QString> ext_signatures;
        QHash<QString, int> ext_signature_offset;
        DataStore * ext;
    };
}

#endif // EXTENSIONS_H
