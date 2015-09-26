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
#include "modules/models/service/search_settings.h"

//#include "model_types.h"

#include "modules/core/core_parts/item_drop_formats.h"
#include "modules/core/media/library.h"

#include "modules/controls/user_dialog_box.h"
#include "modules/plugins/plugins.h"

namespace Model {
    using namespace Core;

    class IModel : public QAbstractItemModel {
        Q_OBJECT
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

        virtual Data::Type playlistType() const = 0;
        virtual bool isRelative() const = 0;

        QVariant data(const QModelIndex & index, int role) const;
        bool setData(const QModelIndex & index, const QVariant & value, int role = Qt::EditRole);

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        bool setHeaderData(int section, Qt::Orientation orientation, const QVariant & value, int role = Qt::EditRole);

        QModelIndex index(IItem * item) const;
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

        int proceedVkList(QJsonArray & collection, Playlist * parent);
        int proceedScList(QJsonArray & collection, Playlist * parent);
        int proceedOdList(QJsonArray & collection, Playlist * parent);
        int proceedGrabberList(Web::SubType wType, QJsonArray & collection, Playlist * parent);

        bool insertRows(const QList<QUrl> & list, int pos, const QModelIndex & parent = QModelIndex());
        virtual bool removeRows(int position, int rows, const QModelIndex & parent = QModelIndex());

        IItem * item(const QModelIndex & index) const;
        template<class T> inline T * item(const QModelIndex & index) const { return dynamic_cast<T *>(item(index)); }

        void shuffle();
        virtual inline QJsonObject toJson() { return rootItem -> toJson(); }

        QModelIndex fromPath(QString path, Direction direction = none);

        inline void setDropKeyboardModifiers(Qt::KeyboardModifiers keyModifiers) { dropKeyModifiers = keyModifiers; }
        Qt::DropActions supportedDropActions() const;
        QStringList mimeTypes() const;
        bool decodeInnerData(int row, int column, const QModelIndex & parent, QDataStream & stream);
        QMimeData * mimeData(const QModelIndexList & indexes) const;
        bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);
        inline QMutex * syncMutex() { return sync; }

        int initiateSearch(SearchRequest & params, Playlist * destination, Playlist * search_source = 0);

        inline virtual bool ignoreListContainUid(QVariant /*uid*/) { return false; }
    public slots:
        inline void itemNotExist(QModelIndex node) { setData(node, ItemState::not_exist, ISTATE); }
        inline void itemNotSupported(QModelIndex node) {
            setData(node, ItemState::not_supported, ISTATE);
            QDesktopServices::openUrl(node.data(IURL).toUrl());
        }
        inline void itemError(QModelIndex node) { setData(node, ItemState::not_supported | ItemState::not_exist, ISTATE); }

        void copyTitleToClipboard(const QModelIndex & index);
        void copyIdsToClipboard(const QModelIndexList & indexes);
        void importIds(QWidget * parent, QStringList ids);

        void markAllAsChecked();
        void markAllAsUnchecked();
//        void removeChecked();

        inline void onUpdateAttr(const QModelIndex ind, int attr, QVariant val) { setData(ind, val, attr); }
        void expanded(const QModelIndex & index);
        void expandeAll();
        void collapsed(const QModelIndex & index);
        void collapseAll();
        virtual inline void refresh(bool /*retryPlaing*/ = false) {} // for web models

    protected slots:
        void finishingItemsAdding();

    signals:
        void spoilNeeded(const QModelIndex & index) const;
        void expandNeeded(const QModelIndex & index) const;
        void itemsCountChanged(int change);

        void moveInBackgroundProcess();
        void moveOutBackgroundProcess();
        void setBackgroundProgress(int percent);

        void moveInProcess();
        void moveOutProcess();
        void setProgress(int percent);
        void setProgress2(int percent);
    protected:
        virtual void recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl /*url*/);
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
}

#endif // MODEL_INTERFACE
