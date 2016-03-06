#ifndef MODEL_INTERFACE
#define MODEL_INTERFACE

#include <qmimedata.h>
#include <qclipboard.h>
#include <QtConcurrent/QtConcurrent> // include async
#include <qfuturewatcher.h> // include async
#include <qdesktopservices.h>

#include "modules/core/core_parts_index.h"
#include "modules/core/web/web_apis.h"
#include "modules/core/misc/file_utils/extensions.h"
#include "modules/core/misc/file_utils/filesystem_watcher.h"
#include "modules/models/service/search_settings.h"

#include "modules/core/core_parts/item_drop_formats.h"
#include "modules/core/media/library.h"

#include "modules/controls/user_dialog_box.h"

#define FUZZY_COMPARITY_PERCENT 80

namespace Models {
    using namespace Core;

    class IModel : public QAbstractItemModel {
        Q_OBJECT

        QModelIndexList dndList;
    public:
        enum Direction {
            none = 0,
            forward = 1,
            backward = 2
        };

        static bool restoreUrl(IItem * itm);

        IModel(QJsonObject * hash, QObject * parent);
        virtual ~IModel();

        inline Playlist * root() { return rootItem; }

        virtual PlaylistType playlistType() const = 0;
        virtual bool isRelative() const = 0;
        inline bool hasFreeMoving() {
            switch(playlistType()) {
                case playlist_tree:
                case playlist_level_tree: return false;
                default: return true;
            }
        }

        void setDNDList(QModelIndexList newDndList) { dndList = newDndList; }

        QVariant data(const QModelIndex & index, int role) const;
        bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        bool setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole);

        inline QModelIndex index(IItem * item) const {
            if (item == rootItem)
                return QModelIndex();
            else
                return createIndex(item -> row(), item -> column(), item);
        }
        inline QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const { return index(row, column, parent, false); }
        QModelIndex index(int row, int column, const QModelIndex & parent, bool orLastChild) const;
        QModelIndex parent(const QModelIndex & index) const;

        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        inline int columnCount(const QModelIndex & /*parent*/ = QModelIndex()) const { return rootItem -> columnCount(); }

        Qt::ItemFlags flags(const QModelIndex & index) const;

        bool insertColumns(int position, int columns, const QModelIndex & parent = QModelIndex());
        bool removeColumns(int position, int columns, const QModelIndex & parent = QModelIndex());

        DropData * threadlyProcessingRowsInsertion(const QList<QUrl> & list, int pos, const QModelIndex & parent);
        bool threadlyInsertRows(const QList<QUrl> & list, int pos, const QModelIndex & parent = QModelIndex());

        static int proceedVkList(const QJsonArray & collection, Playlist * parent, IModel * mdl);
        static int proceedScList(const QJsonArray & collection, Playlist * parent, IModel * mdl);
        static int proceedOdList(const QJsonArray & collection, Playlist * parent, IModel * mdl);
        static int proceedYandexList(const QJsonArray & collection, Playlist * parent);
        static int proceedYoutubeList(const QJsonArray & collection, Playlist * parent);
        static int proceedGrabberList(const DataSubType & wType, const QJsonArray & collection, Playlist * parent);
        static int proceedCue(const QString & path, const QString & name, Playlist * newParent, int insertPos, QHash<QString, bool> & unproc_files, QHash<QString, IItem *> & items);

        bool insertRows(const QList<QUrl> & list, int pos, const QModelIndex & parent = QModelIndex());
        virtual bool removeRows(int position, int rows, const QModelIndex & parent = QModelIndex());

        inline IItem * item(const QModelIndex & index) const {
            if (index.isValid()) {
                IItem * item = static_cast<IItem *>(index.internalPointer());
                if (item)
                    return item;
            }
            return rootItem;
        }
        template<class T> inline T * item(const QModelIndex & index) const { return dynamic_cast<T *>(index.isValid() ? item(index) : rootItem); }

        void shuffle();
        virtual inline QJsonObject toJson() { return rootItem -> toJson(); }

        QModelIndex fromPath(QString path, Direction direction = none);

        inline void setDropKeyboardModifiers(Qt::KeyboardModifiers keyModifiers) { dropKeyModifiers = keyModifiers; }
        Qt::DropActions supportedDropActions() const;
        QStringList mimeTypes() const;
        bool proceedSelfDnd(int row, int column, const QModelIndex & parent);
        bool decodeInnerData(int row, int column, const QModelIndex & parent, QDataStream & stream);
        QMimeData * mimeData(const QModelIndexList & indexes) const;
        bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);
        inline QMutex * syncMutex() { return sync; }

        int initiateSearch(SearchRequest & params, Playlist * destination, Playlist * search_source = 0);
        static int innerSearch(const QString & predicate, Playlist * destination, Playlist * search_source = 0, int count = 9999999);

        inline virtual bool ignoreListContainUid(const QString & /*uid*/) { return false; }
    public slots:
//        inline void itemNotExist(QModelIndex node) { setData(node, ItemState::not_exist, ISTATE); }
//        inline void itemNotSupported(QModelIndex node) {
//            setData(node, ItemState::not_supported, ISTATE);
//            QDesktopServices::openUrl(node.data(IURL).toUrl());
//        }
//        inline void itemError(QModelIndex node) { setData(node, ItemState::not_supported | ItemState::not_exist, ISTATE); }

        void copyTitleToClipboard(const QModelIndex & index);
        void copyIdsToClipboard(const QModelIndexList & indexes);
        void importIds(const QStringList & ids);

        void markAllAsChecked();
        void markAllAsUnchecked();
//        void removeChecked();

        inline void onUpdateAttr(const QModelIndex & ind, int attr, const QVariant & val) { setData(ind, val, attr); }
        void expanded(const QModelIndex & index);
        void expandeAll();
        void collapsed(const QModelIndex & index);
        void collapseAll();
        virtual inline void refresh(bool /*retryPlaing*/ = false) {} // for web models

    protected slots:
        void finishingItemsAdding();

    signals:
        void updateRemovingBlockation(bool isBlocked);
        void spoilNeeded(const QModelIndex & index) const;
        void expandNeeded(const QModelIndex & index) const;
        void collapseNeeded(const QModelIndex & index) const;
        void itemsCountChanged(int change);

        void moveInBackgroundProcess();
        void moveOutBackgroundProcess();
        void setBackgroundProgress(int percent);

        void moveInProcess();
        void moveOutProcess();
        void setProgress(int percent);
        void setProgress2(int percent);
    protected:
        virtual void recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, const QUrl & url);
        virtual void dropProcession(const QModelIndex & parent, int row, const QList<QUrl> & list) = 0;
//        virtual void parseCue(Playlist * parent, QUrl curUri, QUrl cueTargetUri = QUrl()) = 0;

        Qt::KeyboardModifiers dropKeyModifiers;
        QMutex * sync;
        Playlist * rootItem;
    private:
        QFutureWatcher<DropData *> * addWatcher;
    };

    struct modelIndexComparator {
        bool operator()(const QModelIndex & a, const QModelIndex & b) const {
            return a.data(ITREESTR).toString() < b.data(ITREESTR).toString();
        }
    };

    struct itemsComparator {
        bool operator()(const IItem * a, const IItem * b) const {
            return a -> buildTreeStr() > b -> buildTreeStr();
        }
    };
}

#endif // MODEL_INTERFACE

