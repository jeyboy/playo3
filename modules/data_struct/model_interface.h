#ifndef MODEL_INTERFACE
#define MODEL_INTERFACE

#include <qabstractitemmodel.h>
#include <qvariant.h>
#include "misc/file_utils/extensions.h"
#include "item_index.h"

namespace Playo3 {
    class ModelInterface : public QAbstractItemModel {
        Q_OBJECT
    public:
        ModelInterface(QJsonObject * hash, QObject * parent);
        ~ModelInterface();

        QVariant data(const QModelIndex & index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;

        QModelIndex index(ItemInterface * item) const;
        QModelIndex index(int row, int column, const QModelIndex & parent = QModelIndex()) const;
        QModelIndex parent(const QModelIndex & index) const;

        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        inline int columnCount(const QModelIndex & /*parent*/ = QModelIndex()) const { return rootItem -> columnCount(); }

        Qt::ItemFlags flags(const QModelIndex & index) const;
        bool setData(const QModelIndex & index, const QVariant &value, int role = Qt::EditRole);
        bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

        bool insertColumns(int position, int columns, const QModelIndex & parent = QModelIndex());
        bool removeColumns(int position, int columns, const QModelIndex & parent = QModelIndex());
        bool insertRows(int position, int rows, const QModelIndex & parent = QModelIndex());
        bool removeRows(int position, int rows, const QModelIndex & parent = QModelIndex());

    signals:
        itemsCountChanged(int change);
    protected:
        ItemInterface * getItem(const QModelIndex & index) const;
        template<class T> T * getItem(const QModelIndex & index) const {
            return dynamic_cast<T *>(getItem(index));
        }

        ItemInterface * rootItem;
    };

    //        ModelItems::ModelItem * buildPath(QString path);
    //        bool isFolderExist(QString folderName, ModelItems::ModelItem * parent);
    //        ModelItems::ModelItem * addFolder(QString folderPath, QString folderName, ModelItems::ModelItem * parent, QString remoteID = "");
    //        ModelItems::ModelItem * addFolder(QString folder_name, ModelItems::ModelItem * parent, QString remoteID = "");

    //        Qt::DropActions supportedDropActions() const;
    //        QStringList mimeTypes() const;
    //        QMimeData * mimeData(const QModelIndexList &indexes) const;
    //        bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
    //        virtual QModelIndex dropProcession(const QList<QUrl> & list) = 0;

    ////        void refreshItem(ModelItems::ModelItem * item);
    ////        void removeFolderPrebuild(ModelItems::ModelItem * temp);

    ////        virtual WebApi * getApi() { return 0; } //stub
    //    signals:
    //        void itemsCountChanged(int newCount);
    //        void expandNeeded(const QModelIndex &index) const;
    //        void spoilNeeded(const QModelIndex &index) const;
    ////        void showMessage(QString);

    ////        void showSpinner();
    ////        void hideSpinner();

    //        void updated();

    //    public slots:
    ////        void libraryResponse();
    ////        virtual void refresh();
    //        void expanded(const QModelIndex &index);
    //        void collapsed(const QModelIndex &index);

    //    protected:
    //        virtual ModelItems::ModelItem * createItem(QString path, ModelItems::ModelItem * parent);
    //        QString folderName(QFileInfo & info);
}

#endif // MODEL_INTERFACE

