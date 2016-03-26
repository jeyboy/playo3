#ifndef FOLDER_ITEM_H
#define FOLDER_ITEM_H

#include "item.h"
#include <qdir.h>

namespace Core {
    class Playlist : public IItem {
    public:
//        static int restoreItem(int item_type, Playlist * parentFolder, int pos, QVariantMap & attrs);

        inline Playlist(int initState = DEFAULT_ITEM_STATE) : IItem(dt_playlist, 0, initState), filesCount(0) {}
        Playlist(QJsonObject * hash, Playlist * parent = 0, const QJsonValue & childArr = QJsonValue());
        Playlist(const DataSubType & subType, const QString & folderPath, const QString & folderTitle, Playlist * parent = 0, int pos = -1, int initState = DEFAULT_ITEM_STATE);
        Playlist(const DataSubType & subType, const QString & folderTitle, Playlist * parent = 0, int pos = -1, int initState = DEFAULT_ITEM_STATE);
        Playlist(const DataSubType & subType, const QString & folderTitle, Playlist * parent, const QString & id, int pos = -1, int initState = DEFAULT_ITEM_STATE);
        ~Playlist();

        void accumulateUids(QHash<QString, IItem *> & store);
        QStringList childrenUids(int position, int count);

        inline int itemsCountInBranch() const { return filesCount; }
        inline void updateItemsCountInBranch(int offset) { filesCount += offset; }
        void backPropagateItemsCountInBranch(int offset);

        QVariant data(int column) const;

        inline bool isContainer() const { return true; }

        QJsonObject toJson();

        inline QList<IItem *> childrenList() const { return children; }
        inline int childRow(IItem * child) { return children.indexOf(child); }
        inline IItem * child(int row) { return (row < 0 || row >= children.size()) ? 0 : children.value(row); }
        inline bool moveChild(int row, int newRow) {
            if (row < 0 || row >= children.size()) return false;
            if (newRow < 0 || newRow >= children.size()) return false;
            children.move(row, newRow);
            return true;
        }
        inline bool moveChild(IItem * item, int newRow) {
            if (newRow < 0 || newRow >= children.size()) return false;
            children.move(item -> row(), newRow);
            return true;
        }
        virtual inline int childCount() const { return children.count(); }
        inline void declareChild(IItem * child, int pos = -1) {
            if (pos == -1) children.append(child);
            else children.insert(pos, child);
        }
        inline void declareChildren(QList<IItem *> & items) { children.append(items); }
        inline void undeclareChild(IItem * child) {
            if (children.removeOne(child) > 0)
                backPropagateItemsCountInBranch(-((child -> isContainer()) ? ((Playlist *)child) -> childCount() : 1));
        }
        int removeChildren(int position, int count);


        void shuffle();

        void propagatePlaylistSetFlag(ItemStateFlag flag);
        void propagatePlaylistUnsetFlag(ItemStateFlag flag);
        void updateCheckedState(bool checked);
        bool updateCheckedStateByPredicate(ItemStateFlag pred_state);

        Playlist * createPlaylistPath(const DataSubType & subType, const QString & path);
        Playlist * createPlaylist(const DataSubType & subType, const QString & uid, const QString & name, int pos = -1) {
            Playlist * curr = playlists.value(playlistUid(name, uid), 0);
            if (curr) return curr;
            return new Playlist(subType, name, this, uid, pos);
        }
        Playlist * createPlaylist(const DataSubType & subType, const QString & name, QStringList * list = 0, int pos = -1);
//        void addPlaylist(Playlist * node);
        Playlist * findCompatblePlaylist(QStringList * list);
        inline void declarePlaylist(const QString & name, Playlist * playlist) { playlists.insert(name, playlist); }
        inline int undeclarePlaylist(const QString & name) { return playlists.remove(name); }
//        inline bool isContainsFolder(QString name) { return folders.contains(name); }
        inline Playlist * playlist(const QString & name) { return playlists.value(name); }
        inline int playlistsAmount() const { return playlists.size(); }
        inline QList<Playlist *> playlistsList() const { return playlists.values(); }

        void packToStream(QHash<QUrl, int> & urls, QDataStream & stream);

        inline void setParent(Playlist * pNode) {
            if (pNode == _parent) return;
            _parent -> undeclarePlaylist(playlistUid());
            IItem::setParent(pNode);
            pNode -> declarePlaylist(playlistUid(), this);
        }
    protected:
        inline QString playlistUid() const { return playlistUid(title().toString(), id().toString()); }
        inline QString playlistUid(const QString & name, const QString & uid) const { return name + (uid.isEmpty() ? QString() : (QStringLiteral("*") % uid)); }

        QHash<QString, Playlist *> playlists;
        QList<IItem *> children;
        int filesCount;
    };
}

#endif // FOLDER_ITEM_H
