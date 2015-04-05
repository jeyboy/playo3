#ifndef WEB_MODEL_H
#define WEB_MODEL_H

#include "modules/data_struct/model_interface.h"
#include "deleted_list.h"

class WebModel : public IModel, public DeletedList {
    Q_OBJECT

public:
    WebModel(QString uid, QJsonObject * hash = 0, QObject * parent = 0);
    ~WebModel();

    inline QString tabUid() const { return tab_uid; }
//    void VkView::removeItem(ModelItem * item) {
//        QString uid = item -> toUID();
//        if (!uid.isEmpty())
//            ((VkModel *)model) -> addRemovedUID(uid);

//        View::removeItem(item);
//    }
    QJsonObject toJson();
protected:
    void recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl url);
    void dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list);
    int filesRoutine(QFileInfo & currFile, FolderItem * node);
    int filesRoutine(const QList<QUrl> & list, FolderItem * node, int pos = -1);

protected slots:
    void errorReceived(int, QString);
protected:
//    void deleteRemoved(QHash<ModelItem*, QString> & store);
    QString tab_uid;
};


#endif // WEB_MODEL_H
