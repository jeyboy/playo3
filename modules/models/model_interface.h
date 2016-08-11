#ifndef MODEL_INTERFACE
#define MODEL_INTERFACE

#include <qmimedata.h>
#include <qclipboard.h>
#include <QtConcurrent/QtConcurrent> // include async
#include <qfuturewatcher.h> // include async
#include <qdesktopservices.h>

#include "model_settings.h"

#include "modules/core/core_parts/playlist.h"
#include "modules/core/web/web_apis.h"
#include "modules/core/misc/file_utils/extensions.h"
#include "modules/core/misc/file_utils/filesystem_watcher.h"
#include "modules/core/interfaces/search_limits.h"

#include "modules/core/core_parts/item_drop_formats.h"
#include "modules/core/media/library.h"

#include "modules/controls/user_dialog_box.h"

#define MODEL_POSTFIX QStringLiteral("_mdl")

namespace Models {
    using namespace Core;

    class IModel : public QAbstractItemModel {
        Q_OBJECT

        QModelIndexList dndList;
        QFutureWatcher<DropData *> * addWatcher;
    public:
        enum Direction {
            none = 0,
            forward = 1,
            backward = 2
        };

        static bool restoreUrl(IItem * itm);

        IModel(const Params & settings, QJsonObject * hash, QObject * parent);
        virtual ~IModel();

        inline Playlist * root() { return rootItem; }

        inline QString name() {
            QString subject = objectName();
            return subject.mid(0, subject.length() - MODEL_POSTFIX.length());
        }

        inline Params settings() const { return sttngs; }
        inline void setSettings(const Params & newSettings) { sttngs = newSettings; }

        virtual Core::DataSubType playlistType() const = 0;
        virtual bool isRelative() const = 0;
        inline bool hasFreeMoving() {
            switch(playlistType()) {
                case Core::dt_tree:
                case Core::dt_level_tree: return false;
                default: return true;
            }
        }

        void setDNDList(QModelIndexList newDndList) { dndList = newDndList; }

        QVariant data(const QModelIndex & index, int role) const;
        bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        bool setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole);

        void fetchMore(const QModelIndex & parent) {
            Playlist * playlist = item<Playlist>(parent);

            if (playlist -> isLoadable() && !playlist -> is(IItem::flag_in_proc)) {
                playlist -> setStates(IItem::flag_in_proc);

                Func * func = new Func(
                    this,
                    SLOT(finishSetLoading(QJsonValue&, void*)),
                    playlist
                );
                ThreadUtils::obj().run(
                    (IModel *)this,
                    &IModel::proceedLoadable,
                    playlist -> loadableCmd(), func
                );
            }

            QAbstractItemModel::fetchMore(parent);
        }
        bool canFetchMore(const QModelIndex & parent) const {
            if (item(parent) -> isLoadable()) return true;
            return QAbstractItemModel::canFetchMore(parent);
        }

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

        QJsonValue proceedLoadable(const QString & cmd) { return Web::Apis::run(cmd); }

        int proceedBlocks(const DataSubType & wType, const QJsonObject & json, Playlist * parent/*, const DataMediaType & dmtype = dmt_unknow*/);

        int proceedVkList(const QJsonObject & block, Playlist * parent, const DataMediaType & fdmtype = dmt_unknow, const DataSubType & wType = dt_site_vk);
        int proceedVkSet(const QJsonObject & block, Playlist * parent, const DataMediaType & fdmtype = dmt_unknow, const DataSubType & wType = dt_site_vk);

        int proceedScList(const QJsonObject & collection, Playlist * parent, const DataMediaType & dmtype = dmt_unknow, const DataSubType & wType = dt_site_sc);
        int proceedScSet(const QJsonObject & collection, Playlist * parent, const DataMediaType & dmtype = dmt_unknow, const DataSubType & wType = dt_site_sc);

        int proceedOdList(const QJsonObject & collection, Playlist * parent, const DataMediaType & dmtype = dmt_unknow, const DataSubType & wType = dt_site_od);
        int proceedYandexList(const QJsonObject & collection, Playlist * parent, const DataMediaType & dmtype = dmt_unknow, const DataSubType & wType = dt_site_yandex);
        int proceedYoutubeList(const QJsonObject & collection, Playlist * parent, const DataMediaType & dmtype = dmt_unknow, const DataSubType & wType = dt_site_youtube);
        int proceedGrabberList( const QJsonObject & collection, Playlist * parent, const DataMediaType & dmtype, const DataSubType & wType);

        int proceedCue(const QString & path, const QString & name, Playlist * newParent, int insertPos, QHash<QString, bool> & unproc_files, QHash<QString, IItem *> & items);

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

        int search(SearchLimit & params, Playlist * destination, Playlist * search_source = 0);
        int search(const QString & predicate, Playlist * destination, Playlist * search_source = 0, int count = 9999999);

        inline virtual bool ignoreListContainUid(const QString & /*uid*/) { return false; }
    public slots:
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
        virtual inline void refresh() {} // for web models

    protected slots:
        void finishingItemsAdding();
        void finishSetLoading(QJsonValue&, void*);

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
        Params sttngs;
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

