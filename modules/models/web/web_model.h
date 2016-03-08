#ifndef WEB_MODEL_H
#define WEB_MODEL_H

#include "modules/models/list_model.h"
#include "ignore_list.h"

#define UPDATE_INTERVAL 10000

namespace Models {
    using namespace Core::Web;

    class WebModel : public ListModel, public IgnoreList {
        Q_OBJECT
    public:
        inline WebModel(const QString & uid, QJsonObject * hash = 0, QObject * parent = 0) :
            ListModel(hash, parent), IgnoreList(hash), tab_uid(uid) {  }
        inline virtual ~WebModel() {}

        inline bool isRelative() const { return false; }
        virtual ISearchable * api() = 0;
        inline QString tabUid() const { return tab_uid; }

        bool removeRows(int position, int rows, const QModelIndex & parent);
        void toJson(QJsonObject & res);

        inline bool ignoreListContainUid(const QString & uid) { return IgnoreList::ignoreListContainUid(uid); } // overriding for model_interface
    protected:
        int filesRoutine(const QString & filePath, Playlist * node);
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
