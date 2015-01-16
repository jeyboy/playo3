#ifndef EXTENSIONS_H
#define EXTENSIONS_H

//#include <QHash>
#include <QDir>
#include "data_store.h"

namespace Playo3 {
    class Extensions {
    public:
        static inline QString folderName(QFileInfo & info) {
            QString name = info.dir().dirName();
            if (name.isEmpty())
                name = info.dir().path().split('/').first();
            return name;
        }

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
        Extensions() {
            ext = new DataStore("extensions.json");

            if (ext -> state) {
                activeFilter = ext -> read("active").toString("all");
                QJsonObject obj = ext -> read("filters").toObject();
                foreach (QString key, obj.keys()) {
                    filters.insert(key, obj.value(key).toVariant().value<QStringList>());
                }
            } else {
                QStringList commonfiltersList;
                commonfiltersList << "*.*";
                filters.insert("all", commonfiltersList);

    //            QStringList filtersList;
    //            filtersList << "*.wav"
    //                        << "*.aiff"
    //                        << "*.aif"
    //                        << "*.mp3"
    //                        << "*.mp2"
    //                        << "*.mp1"
    //                        << "*.ogg"
    //                        << "*.wma"
    //                        << "*.mpc"
    //                        << "*.aac"
    //                        << "*.alac"
    //                        << "*.ac3"
    //                        << "*.wv"
    //                        << "*.ape"
    //                        << "*.flac";

    //            filters.insert("audio", filtersList);
                activeFilter = "all";
            }
        }

        static Extensions *self;

        QString activeFilter;
        QHash<QString, QStringList> filters;
        DataStore * ext;
    };
}

#endif // EXTENSIONS_H
