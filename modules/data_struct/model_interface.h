#ifndef MODEL_INTERFACE
#define MODEL_INTERFACE

#include <qmimedata.h>
#include <qabstractitemmodel.h>
//#include <qvariant.h>
#include "misc/file_utils/extensions.h"
#include "item_index.h"
#include "container_types.h"

namespace Playo3 {
    struct InnerData {
        InnerData() {}

        QUrl url;
        QVariantMap attrs;
        QModelIndex eIndex;
        int eRow, dRow;
    };

    #define DROP_OUTER_FORMAT "text/uri-list"
    #define DROP_INNER_FORMAT "application/x-qabstractitemmodeldatalist"

    class ModelInterface : public QAbstractItemModel {
        Q_OBJECT
    public:
        ModelInterface(QJsonObject * hash, QObject * parent);
        ~ModelInterface();

        virtual ContainerType containerType() const = 0;

        QVariant data(const QModelIndex & index, int role) const;
        bool setData(const QModelIndex & index, const QVariant &value, int role = Qt::EditRole);

        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

        QModelIndex index(ItemInterface * item) const;
        inline QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const { return index(row, column, parent, false); }
        QModelIndex index(int row, int column, const QModelIndex & parent, bool orLastChild) const;
        QModelIndex parent(const QModelIndex & index) const;

        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        inline int columnCount(const QModelIndex & /*parent*/ = QModelIndex()) const { return rootItem -> columnCount(); }

        Qt::ItemFlags flags(const QModelIndex & index) const;

        bool insertColumns(int position, int columns, const QModelIndex & parent = QModelIndex());
        bool removeColumns(int position, int columns, const QModelIndex & parent = QModelIndex());
//        bool insertRows(int position, int rows, const QModelIndex & parent = QModelIndex());
        virtual bool insertRows(const QList<QUrl> & list, int pos, const QModelIndex & parent = QModelIndex());
        bool removeRows(int position, int rows, const QModelIndex & parent = QModelIndex());

        ItemInterface * item(const QModelIndex & index) const;

        void shuffle();
        inline QJsonObject toJson() { return rootItem -> toJson(); }

        inline void setDropKeyboardModifiers(Qt::KeyboardModifiers keyModifiers) { dropKeyModifiers = keyModifiers; }
        Qt::DropActions supportedDropActions() const;
        QStringList mimeTypes() const;
        bool decodeInnerData(int row, int column, const QModelIndex & parent, QDataStream & stream);
        QMimeData * mimeData(const QModelIndexList & indexes) const;
        bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);

    public slots:
//        void libraryResponse();
//        virtual void refresh();
        void expanded(const QModelIndex & index);
        void expandeAll();
        void collapsed(const QModelIndex & index);
        void collapseAll();

    signals:
        void spoilNeeded(const QModelIndex & index) const;
        void expandNeeded(const QModelIndex & index) const;
        void itemsCountChanged(int change);
    protected:
        void proceedMimeDataIndex(const QModelIndex ind, QList<QUrl> & urls, QDataStream & stream) const;
        virtual void recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl /*url*/);
        QModelIndex fromPath(QString path);
        virtual void dropProcession(const QModelIndex & parent, int row, const QList<QUrl> & list) = 0;

        template<class T> T * item(const QModelIndex & index) const {
            return dynamic_cast<T *>(item(index));
        }

        Qt::KeyboardModifiers dropKeyModifiers;
        FolderItem * rootItem;
    };

    ////        void refreshItem(ModelItems::ModelItem * item);
    ////        void removeFolderPrebuild(ModelItems::ModelItem * temp);

    ////        virtual WebApi * getApi() { return 0; } //stub
    //    signals:
    ////        void showMessage(QString);

    ////        void showSpinner();
    ////        void hideSpinner();

    //        void updated();
}

#endif // MODEL_INTERFACE

