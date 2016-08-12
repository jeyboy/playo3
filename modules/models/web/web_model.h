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
        inline WebModel(const Params & settings, QJsonObject * hash = 0, QObject * parent = 0) :
            ListModel(settings, hash, parent), IgnoreList(hash) {  }
        inline virtual ~WebModel() {}

        inline bool isRelative() const { return false; }
        virtual ISource * api() const = 0;
        inline Core::DataSubType playlistType() const { return api() -> sourceType(); }

        bool removeRows(int position, int rows, const QModelIndex & parent);
        QJsonObject toJson();

        inline bool ignoreListContainUid(const QString & uid) { return IgnoreList::ignoreListContainUid(uid); } // overriding for model_interface
    protected:
        int filesRoutine(const QString & filePath, Playlist * node, QHash<QString, bool> & unproc_files, QHash<QString, IItem *> & items);
        int filesRoutine(const QList<QUrl> & list, Playlist * node, int pos = -1);

    signals:
        void refreshNeeded();

    protected slots:
        void errorReceived(int, QString);
    };
}

#endif // WEB_MODEL_H
