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
#include "modules/models/service/removed_items.h"

#include "modules/core/core_parts/item_drop_formats.h"
#include "modules/core/media/library.h"

#include "modules/controls/user_dialog_box.h"

#define MODEL_POSTFIX LSTR("_mdl")

namespace Models {
    using namespace Core;

    class IModel : public QAbstractItemModel {
        Q_OBJECT

        bool block_fetching;
        QModelIndexList dnd_list;
        QFutureWatcher<DropData *> * add_watcher;
        RemovedItems deleted_items;
    public:
        enum Direction {
            none = 0,
            forward = 1,
            backward = 2
        };

        static bool restoreUrl(IItem * itm);

        IModel(const Params & settings, QJsonObject * hash, QObject * parent);
        virtual ~IModel();

        inline Playlist * root() { return root_item; }

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

        void setDNDList(QModelIndexList new_dnd_list) { dnd_list = new_dnd_list; }

        QVariant data(const QModelIndex & index, int role) const;
        bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        bool setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole);

        void fetchMore(const QModelIndex & parent) { fetchMore(parent, true); }
        void fetchMore(const QModelIndex & parent, bool in_background) {
            Playlist * playlist = item<Playlist>(parent);

            if (playlist -> hasMoreItems() && !playlist -> is(IItem::flag_in_proc)) {
                emit moveInBackgroundProcess();

                QVariant cmds = playlist -> isLoadable() ?  playlist -> loadableCmd() :  playlist -> fetchableCmd();
                QVariantMap cmds_list = VAR_TO_MAP(cmds);

                for(QVariantMap::Iterator cmd = cmds_list.begin(); cmd != cmds_list.end(); cmd++) {
                    playlist -> setStates(IItem::flag_in_proc);
                    QPair<Playlist *, QString> * params = new QPair<Playlist *, QString>(playlist, cmd.key());

                    if (in_background) {
                        Func * func = new Func(
                            this,
                            SLOT(finishSetLoading(const QJsonValue&, void*)),
                            params
                        );

                        ThreadUtils::obj().run(
                            (IModel *)this,
                            &IModel::proceedLoadable,
                            cmd.key(), func
                        );
                    }
                    else
                        finishSetLoading(proceedLoadable(cmd.key()), params);
                }
            }

            QAbstractItemModel::fetchMore(parent);
        }
        bool canFetchMore(const QModelIndex & parent) const {
            if (block_fetching) return false;

            IItem * itm = item(parent);

            if (itm -> isLoadable()) return true;

            //INFO: fetch only root items at this time
            if (itm -> isFetchable() && itm == root_item && !itm -> is(IItem::flag_in_proc)) {
                itm -> setStates(IItem::flag_in_proc);
                emit fetchNeeded(parent);
            }

            return QAbstractItemModel::canFetchMore(parent);
        }

        inline bool indexPersisted(const QModelIndex & ind) {
            return !deleted_items.containsItem(ind.internalPointer());
        }
        inline QModelIndex index(IItem * item) const {
            if (item == root_item)
                return QModelIndex();
            else
                return createIndex(item -> row(), item -> column(), item);
        }
        inline QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const { return index(row, column, parent, false); }
        QModelIndex index(int row, int column, const QModelIndex & parent, bool orLastChild) const;
        QModelIndex parent(const QModelIndex & index) const;

        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        inline int columnCount(const QModelIndex & /*parent*/ = QModelIndex()) const { return root_item -> columnCount(); }

        Qt::ItemFlags flags(const QModelIndex & index) const;

        bool insertColumns(int position, int columns, const QModelIndex & parent = QModelIndex());
        bool removeColumns(int position, int columns, const QModelIndex & parent = QModelIndex());

        DropData * threadlyProcessingRowsInsertion(const QList<QUrl> & list, int pos, const QModelIndex & parent);
        bool threadlyInsertRows(const QList<QUrl> & list, int pos, const QModelIndex & parent = QModelIndex());

        QJsonValue proceedLoadable(const QString & cmd) { return Web::Apis::run(cmd); }

        int proceedBlocks(const QJsonArray & blocks, Playlist * parent, const bool & is_loadable = false);

        int proceedVkList(const QJsonObject & block, Playlist * parent, int & update_amount, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype = dmt_unknow, const DataSubType & wType = dt_web_vk);
//        int proceedVkSet(const QJsonObject & block, Playlist * parent, int & update_amount, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype = dmt_unknow, const DataSubType & wType = dt_web_vk);

        int proceedScList(const QJsonObject & collection, Playlist * parent, int & update_amount, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype = dmt_unknow, const DataSubType & wType = dt_web_sc);
        int proceedScSet(const QJsonObject & collection, Playlist * parent, int & update_amount, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype = dmt_unknow, const DataSubType & wType = dt_web_sc);

        int proceedOdList(const QJsonObject & collection, Playlist * parent, int & update_amount, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype = dmt_unknow, const DataSubType & wType = dt_web_od);
        int proceedOdSet(const QJsonObject & collection, Playlist * parent, int & update_amount, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype = dmt_unknow, const DataSubType & wType = dt_web_od);

        int proceedYandexList(const QJsonObject & collection, Playlist * parent, int & update_amount, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype = dmt_unknow, const DataSubType & wType = dt_web_yandex);
        int proceedYandexSet(const QJsonObject & collection, Playlist * parent, int & update_amount, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype = dmt_unknow, const DataSubType & wType = dt_web_od);

        int proceedYoutubeList(const QJsonObject & collection, Playlist * parent, int & update_amount, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype = dmt_unknow, const DataSubType & wType = dt_web_youtube);

        int proceedGrabberList( const QJsonObject & collection, Playlist * parent, int & update_amount, QHash<Playlist *, QHash<QString, IItem *> > & stores, const DataMediaType & fdmtype, const DataSubType & wType);

        int proceedCue(const QString & path, const QString & name, Playlist * newParent, int insertPos, QHash<QString, bool> & unproc_files, QHash<QString, IItem *> & items);

        bool insertRows(const QList<QUrl> & list, int pos, const QModelIndex & parent = QModelIndex());
        virtual bool removeRows(int position, int rows, const QModelIndex & parent = QModelIndex());

        inline IItem * item(const QModelIndex & index) const {
            if (index.isValid()) {
                IItem * item = static_cast<IItem *>(index.internalPointer());
                if (item)
                    return item;
            }
            return root_item;
        }
        template<class T> inline T * item(const QModelIndex & index) const { return dynamic_cast<T *>(index.isValid() ? item(index) : root_item); }

        void blockFetching(bool blocked = true) { block_fetching = blocked; }

        void shuffle();
        virtual inline QJsonObject toJson() { return root_item -> toJson(); }

        QModelIndex fromPath(QString path, Direction direction = none);

        inline void setDropKeyboardModifiers(Qt::KeyboardModifiers key_modifiers) { drop_key_modifiers = key_modifiers; }
        Qt::DropActions supportedDropActions() const;
        QStringList mimeTypes() const;
        bool proceedSelfDnd(int row, int column, const QModelIndex & parent);
        bool decodeInnerData(int row, int column, const QModelIndex & parent, QDataStream & stream);
        QMimeData * mimeData(const QModelIndexList & indexes) const;
        bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);
        inline QMutex * syncMutex() { return sync; }

        int search(const SearchLimit & params, Playlist * destination, Playlist * search_source = 0);
        int search(const QString & predicate, Playlist * destination, Playlist * search_source = 0, int count = 9999999);

        inline virtual bool ignoreListContainUid(const QString & /*uid*/) { return false; } // for web models
        inline virtual bool ignoreListHasItems() { return false; } // for web models
    public slots:
        void updateIcon() {
            emit changeIco(
                root_item -> hasMoreItems() ? Settings::currentStyle -> loadableTabIcon() : QImage()
            );
        }

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
        virtual inline void clearIgnoreList() {} // for web models

    protected slots:
        void finishingItemsAdding();
        void finishSetLoading(const QJsonValue&, void*);

    signals:
        void updateRemovingBlockation(bool isBlocked);
        void spoilNeeded(const QModelIndex & index) const;
        void expandNeeded(const QModelIndex & index) const;
        void collapseNeeded(const QModelIndex & index) const;
        void fetchNeeded(const QModelIndex & index) const;
        void itemsCountChanged(int change);

        void changeIco(const QImage &);
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

        Qt::KeyboardModifiers drop_key_modifiers;
        QMutex * sync;
        Playlist * root_item;
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

