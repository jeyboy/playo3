#ifndef WEB_MODEL_H
#define WEB_MODEL_H

#include "modules/data_struct/model_interface.h"
#include "ignore_list.h"

class WebApi;
namespace Playo3 {
    class WebModel : public IModel, public IgnoreList {
        Q_OBJECT

    public:
        WebModel(QString uid, QJsonObject * hash = 0, QObject * parent = 0);
        ~WebModel();

        virtual WebApi * api() = 0;
        inline QString tabUid() const { return tab_uid; }

        bool removeRows(int position, int rows, const QModelIndex & parent);
        QJsonObject toJson();
    protected:
        void recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl url);
        void dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list);
        int filesRoutine(QFileInfo & currFile, FolderItem * node);
        int filesRoutine(const QList<QUrl> & list, FolderItem * node, int pos = -1);

    signals:
        void refreshNeeded();

    protected slots:
            void errorReceived(int, QString);
    protected:
        QString tab_uid;
    };
}

#endif // WEB_MODEL_H
