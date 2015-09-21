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
        Playlist * createPlaylist(const QString & name, QStringList * list = 0, int pos = -1);
        Playlist * findCompatblePlaylist(QStringList * list);
        inline void declarePlaylist(const QString & name, Playlist * playlist) { playlists.insert(name, playlist); }
        inline int undeclarePlaylist(const QString & name) { return playlists.remove(name); }
//        inline bool isContainsFolder(QString name) { return folders.contains(name); }
        inline Playlist * findPlaylist(const QString & name) { return playlists.value(name); }
        inline int playlistsAmount() const { return playlists.size(); }
        inline QList<Playlist *> playlistsList() const { return playlists.values(); }

        void packToStream(QHash<QUrl, int> & urls, QDataStream & stream);
    protected:
        inline QString playlistUid() const { return playlistUid(title().toString(), uid().toString()); }
        inline QString playlistUid(const QString & name, const QString & uid) const { return name + (uid.isEmpty() ? QString() : (QStringLiteral("*") % uid)); }

        QHash<QString, Playlist *> playlists;
        QList<IItem *> children;
        int filesCount;
    };
}

#endif // FOLDER_ITEM_H
