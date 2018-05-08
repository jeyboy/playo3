#ifndef EXTENSIONS_H
#define EXTENSIONS_H

#include <qdir.h>
#include <qstringbuilder.h>

#include "data_store.h"

#include "modules/core/interfaces/singleton.h"

#define JSON_KEY QLatin1String("filters")
#define JSON_ACTIVE_KEY QLatin1String("active")
#define DEFAULT_ACTIVE QLatin1String("all")
#define MUSIC_PRESET QLatin1String("music")

namespace Core {
    class Extensions : public Singleton<Extensions> {
    public:
        static QString dot_cue_ext;
        static QString cue_ext;

        static QString folderName(QFileInfo & info);
        ~Extensions();

        bool extractExtension(QString & fileName, QString & sExt, bool extract = true);
        bool restoreExtension(QString & file_path, QString & restoredExt);

        bool respondToExtension(const QString & ext) const;
        QStringList activeFilterList() const;
        QStringList filterList(const QString & name) const;
        void filterListUpdate(const QString & name, QStringList & list);
        void addNewPreset(QString name);
        void removePreset(QString name);

        inline void setActiveFilterName(const QString & name) { activeFilter = name; }
        inline QString activeFilterName() const { return activeFilter; }
        inline QStringList presetsList() const { return QStringList(filters.keys()); }

    private:
        Extensions(); friend class Singleton<Extensions>;
        void initSignatures();
        void initMusicExtensions();

        QString activeFilter;
        QHash<QString, QStringList> filters;
        QHash<QString, QString> ext_signatures;
        QHash<QString, int> ext_signature_offset;
        DataStore * ext;
    };
}

#endif // EXTENSIONS_H
