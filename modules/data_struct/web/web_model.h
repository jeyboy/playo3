#ifndef WEB_MODEL_H
#define WEB_MODEL_H

#include "modules/data_struct/model_interface.h"
#include "ignore_list.h"

#define UPDATE_INTERVAL 10000

class WebApi;
namespace Playo3 {
    class WebModel : public IModel, public IgnoreList {
        Q_OBJECT
    public:
        inline WebModel(QString uid, const Json::Obj & hash = Json::Obj(), QObject * parent = 0) :
            IModel(hash, parent), IgnoreList(hash), tab_uid(uid)
        { lastRefresh = QDateTime::currentMSecsSinceEpoch() - UPDATE_INTERVAL; }
        inline virtual ~WebModel() {}

        inline bool isRelative() const { return false; }
        virtual WebApi * api() = 0;
        inline QString tabUid() const { return tab_uid; }

        bool removeRows(int position, int rows, const QModelIndex & parent);
        Json::Obj toJson();
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
        qint64 lastRefresh;
    };
}

#endif // WEB_MODEL_H
