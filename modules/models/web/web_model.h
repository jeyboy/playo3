#ifndef WEB_MODEL_H
#define WEB_MODEL_H

#include "modules/models/list_model.h"
#include "ignore_list.h"

namespace Models {
    using namespace Core::Web;

    class WebModel : public ListModel, public IgnoreList {
        Q_OBJECT
    public:
        inline WebModel(const Params & settings, QJsonObject * hash = 0, QObject * parent = 0) :
            ListModel(settings, hash, parent), IgnoreList(hash) {  }
        inline virtual ~WebModel() {}

        inline bool isRelative() const { return false; }
        ISource * api() const { return Web::Apis::source(sttngs.data_type); }
        inline Core::DataSubType playlistType() const { return api() -> sourceType(); }

        bool removeRows(int position, int rows, const QModelIndex & parent);
        QJsonObject toJson();

        inline bool ignoreListContainUid(const QString & uid) { return IgnoreList::ignoreListContainUid(uid); } // overriding for model_interface
        bool ignoreListHasItems() { return !ignoreListIsEmpty(); }
    protected:
        int filesRoutine(const QString & filePath, Playlist * node, QHash<QString, bool> & unproc_files, QHash<QString, IItem *> & items);
        int filesRoutine(const QList<QUrl> & list, Playlist * node, int pos = -1);

    signals:
        void refreshNeeded();

    public slots:
        void refresh();
        void clearIgnoreList() { ignoreListClear(); }
    protected slots:
        void proceedJson(QJsonValue &);
        void errorReceived(int, QString);
    };
}

#endif // WEB_MODEL_H
