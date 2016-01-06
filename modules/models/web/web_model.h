#ifndef WEB_MODEL_H
#define WEB_MODEL_H

#include "modules/models/model_interface.h"
#include "ignore_list.h"

#define UPDATE_INTERVAL 10000

namespace Models {
    using namespace Core::Web;

    class WebModel : public IModel, public IgnoreList {
        Q_OBJECT
    public:
        inline WebModel(const QString & uid, QJsonObject * hash = 0, QObject * parent = 0) :
            IModel(hash, parent), IgnoreList(hash), tab_uid(uid) {  }
        inline virtual ~WebModel() {}

        inline bool isRelative() const { return false; }
        virtual ISearchable * api() = 0;
        inline QString tabUid() const { return tab_uid; }

        bool removeRows(int position, int rows, const QModelIndex & parent);
        QJsonObject toJson();

        inline bool ignoreListContainUid(const QString & uid) { return IgnoreList::ignoreListContainUid(uid); } // overriding for model_interface
    protected:
        void recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl url);
        void dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list);
        int filesRoutine(QFileInfo & currFile, Playlist * node);
        int filesRoutine(const QList<QUrl> & list, Playlist * node, int pos = -1);

    signals:
        void refreshNeeded();

    protected slots:
            void errorReceived(int, QString);
    protected:
        QString tab_uid;
    };
}

#endif // WEB_MODEL_H
