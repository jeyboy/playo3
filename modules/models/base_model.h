#ifndef BASE_MODEL_H
#define BASE_MODEL_H

#include <QObject>
#include <QAbstractItemModel>

#include "../model_items/model_item.h"
#include "../controls/control_json_fields.h"

namespace Models {
    class BaseModel : public QAbstractItemModel {
        Q_OBJECT
    public:
        Model(QJsonObject *hash = 0, QObject *parent = 0);
        ~Model();

        QVariant data(const QModelIndex &index, int role) const;
        bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
        QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
        bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole);

        Qt::ItemFlags flags(const QModelIndex &index) const;
        QModelIndex parent(const QModelIndex &index) const;
        QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
//        QModelIndex index(ModelItem * item) const;

        int itemsCount() const;

        int columnCount(const QModelIndex &parent = QModelIndex()) const;

        int rowCount(const QModelIndex &parent = QModelIndex()) const;
//        void appendRow(ModelItems::ModelItem * item);
        bool removeRow(int row, const QModelIndex &parent = QModelIndex());
        bool removeRows(int position, int rows, const QModelIndex &parent = QModelIndex());

        void clearAll(bool emitChanges = false);

        ModelItems::ModelItem *getItem(const QModelIndex &index) const;
    //    template<class T> T *getItem(const QModelIndex &index) const;
        ModelItems::ModelItem * root() const;

        ModelItems::ModelItem * buildPath(QString path);
        bool isFolderExist(QString folderName, ModelItem * parent);
        ModelItems::ModelItem * addFolder(QString folderPath, QString folderName, ModelItem * parent, QString remoteID = "");
        ModelItems::ModelItem * addFolder(QString folder_name, ModelItem * parent, QString remoteID = "");

        Qt::DropActions supportedDropActions() const;
        QStringList mimeTypes() const;
        QMimeData * mimeData(const QModelIndexList &indexes) const;
        bool dropMimeData(const QMimeData *data, Qt::DropAction action, int row, int column, const QModelIndex &parent);
        virtual QModelIndex dropProcession(const QList<QUrl> & list) = 0;

//        void refreshItem(ModelItems::ModelItem * item);
//        void removeFolderPrebuild(ModelItems::ModelItem * temp);

//        virtual WebApi * getApi() { return 0; } //stub
    signals:
        void itemsCountChanged(int newCount);
        void expandNeeded(const QModelIndex &index) const;
        void spoilNeeded(const QModelIndex &index) const;
//        void showMessage(QString);

//        void showSpinner();
//        void hideSpinner();

        void updated();

    public slots:
//        void libraryResponse();
//        virtual void refresh();
        void expanded(const QModelIndex &index);
        void collapsed(const QModelIndex &index);

    protected:
        virtual ModelItems::ModelItem * createItem(QString path, ModelItems::ModelItem * parent);
        QString folderName(QFileInfo & info);

        int count;
        ModelItems::ModelItem * rootItem;
    };
}

#endif // BASE_MODEL_H
