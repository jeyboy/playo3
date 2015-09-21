#ifndef FOLDER_ITEM_H
#define FOLDER_ITEM_H

#include "item_interface.h"
#include <qdir.h>
#include <qdatetime.h>

namespace Core {
    class Playlist : public IItem {
    public:
        static int restoreItem(int item_type, Playlist * parentFolder, int pos, QVariantMap & attrs);

        inline Playlist(int initState = DEFAULT_PLAYLIST_STATE) : IItem(0, initState), filesCount(0) {}
        Playlist(QJsonObject * hash, Playlist * parent = 0);
        Playlist(QString folderPath, QString folderTitle, Playlist * parent = 0, int pos = -1, int initState = DEFAULT_PLAYLIST_STATE);
        Playlist(QString folderTitle, Playlist * parent = 0, int pos = -1, int initState = DEFAULT_PLAYLIST_STATE);
        Playlist(QString folderTitle, Playlist * parent, QString uid, int pos = -1, int initState = DEFAULT_PLAYLIST_STATE);
        virtual ~Playlist();

        void linkNode(Playlist * node);

        void accumulateUids(QHash<QString, IItem *> & store);
        QVariantList childrenUids(int position, int count);

        inline int itemsCountInBranch() const { return filesCount; }
        inline void updateItemsCountInBranch(int offset) { filesCount += offset; }
        void backPropagateItemsCountInBranch(int offset);

        QVariant data(int column) const;

        inline void openLocation() { QDesktopServices::openUrl(toUrl()); }
        inline int itemType() const { return PLAYLIST; }
        bool removePhysicalObject();

        inline bool isExist() const { return QDir(fullPath()).exists(); }
        inline bool isContainer() const { return true; }

        QJsonObject toJson();

        inline QList<IItem *> childrenList() const { return children; }
        inline int childRow(IItem * child) { return children.indexOf(child); }
        inline IItem * child(int row) { return (row < 0 || row >= children.size()) ? 0 : children.value(row); }
        virtual inline int childCount() const { return children.count(); }
        inline void declareChild(IItem * child) { children.append(child); }
        inline void declareChildren(QList<IItem *> & items) { children.append(items); }
        inline void declareChild(int pos, IItem * item) { children.insert(pos, item); }
        int removeChildren(int position, int count);
        inline void removeYouself() { if (_parent) _parent -> removeChildren(row(), 1); }


        void shuffle();

        void propagatePlaylistSetFlag(ItemStateFlag flag);
        void propagatePlaylistUnsetFlag(ItemStateFlag flag);
        void updateCheckedState(bool checked);
        bool updateCheckedStateByPredicate(ItemStateFlag pred_state);

        Playlist * createPlaylistPath(QString path);
        template<class T> T * createPlaylist(const QString & uid, const QString & name, int pos = -1) {
            T * curr = reinterpret_cast<T *>(playlists.value(palylistUid(name, uid), 0));
            if (curr) return curr;
            return new T(uid, name, this, pos);
        }
        PlaylistItem * createFolder(QString name, QStringList * list = 0, int pos = -1);
        PlaylistItem * findNearestFolder(QStringList * list);
        inline void declareFolder(QString name, PlaylistItem * folder) { folders.insert(name, folder); }
        inline int undeclareFolder(QString name) { return folders.remove(name); }
//        inline bool isContainsFolder(QString name) { return folders.contains(name); }
        inline PlaylistItem * PlaylistItem(QString name) { return folders.value(name); }
        inline int foldersAmount() const { return folders.size(); }
        inline QList<PlaylistItem *> folderChildren() const { return folders.values(); }

        void packToStream(QHash<QUrl, int> & urls, QDataStream & stream);
    protected:
        inline QString folderUid() const { return folderUid(title().toString(), uid().toString()); }
        inline QString folderUid(QString name, QString uid) const { return name + (uid.isEmpty() ? "" : ("*" + uid)); }

        QHash<QString, Playlist *> playlists;
        QList<IItem *> children;
        int filesCount;
    };
}

#endif // FOLDER_ITEM_H
