#ifndef ITEM_INTERFACE
#define ITEM_INTERFACE

#include <qlist.h>
#include <qdesktopservices.h>
#include <qurl.h>
#include <qstringbuilder.h>

#include "core_part_types.h"
#include "part_mixes/item_fields.h"

#include "modules/core/web/apis/social/soundcloud_api.h"
#include "modules/core/misc/file_utils/icon_provider.h"

#define REMOTE_DND_URL QUrl::fromLocalFile("REMOTE:/")

namespace Core {
    class Playlist;

    class IItem : public ItemFields {
        void connectToSource(IItem * newSource) {
            qSwap(newSource -> attrs, attrs);
            swap(newSource);
        }
    public:
        IItem(Playlist * parent = 0, int initState = DEFAULT_ITEM_STATE);
        IItem(Playlist * parent, QVariantMap & hash, int pos = -1);
        IItem(Playlist * parent, QJsonObject * hash);
        IItem(Playlist * parent, const QString & title, int pos = -1, int initState = DEFAULT_ITEM_STATE);

        inline virtual ~IItem() {
            if (has(flag_mark_on_removing) && !isParted())
                removePhysicalObject();
        }

        virtual QJsonObject toJson();

        QString fullPath() const {
            QString path_buff = path().toString();
            #ifdef Q_OS_LINUX
                path_buff = '/' % path_buff;
            #endif
            return path_buff;
        }

        void openLocation();
        bool removePhysicalObject() {
            switch(dataType()) {
                case dt_local:
                    return QFile::remove(fullPath());
                case dt_local_cue: // this required on some additional checks
                default: return false;
            }


        }
        bool isExist() const {
            switch(dataType()) {
                case dt_local:
                case dt_local_cue:
                    return QFile::exists(fullPath());
                default: return true;
            }
        }

        inline int column() const { return 0; }
        int row() const;

        virtual inline int itemsCountInBranch() const { return 1; }

        inline virtual bool isContainer() const { return false; }

        inline bool isPlayable() const {
            if (isContainer()) return false;

            bool showBatch = Settings::obj().isCheckboxShow();
            return !showBatch || (showBatch && has(flag_checked));
        }

        QUrl toUrl() const {
            if (isRemote()) {
                QUrl url = QUrl(path().toString());
                switch(dataType()) {
                    case dt_site_sc:
                        url.setQuery(Web::Soundcloud::Api::obj().genDefaultParams());
                    default: ;
                }
                return url;
            } else
                return QUrl::fromLocalFile(fullPath());
        }
        QString buildTreePath() const;
        QString buildTreeStr() const; // used for tree sorting on removing

        virtual QVariant data(int column) const;
        bool setData(int column, const QVariant & value);

        inline Playlist * parent() { return _parent; }

        inline virtual int columnCount() const { return 1; }

        inline virtual void updateCheckedState(bool setChecked) {
            if (setChecked) set(flag_checked); else unset(flag_checked);
        }

        inline virtual bool updateCheckedStateByPredicate(ItemStateFlag pred_state) {
            bool valid = has(pred_state);
            updateCheckedState(valid);
            return valid;
        }

        void removeYouself();
        virtual void packToStream(QHash<QUrl, int> & urls, QDataStream & stream);

//        inline bool respondTo(QString & predicate) {
////            QRegularExpression regex(predicate, QRegularExpression::CaseInsensitiveOption);
////            QRegularExpressionMatch m = regex.match(it.data().toString(), 0, QRegularExpression::PartialPreferFirstMatch);
////            return m.hasMatch();

//            return /*predicate.isEmpty() ||*/ title().toString().contains(predicate, Qt::CaseInsensitive);
//        }

        virtual void setParent(Playlist * pNode, int pos = -1);

        inline void fixSourceLimit() { activeSourceIndexLimit = activeSourceIndex(); }
        bool addSource(QJsonObject * hash);
        bool addSource(IItem * newSource, bool setAsMain = false, bool checkExistance = true);
        bool useNextSource();
        IItem * activeSourceItem() const;
    protected:
        bool eqlByLocation(IItem * itm) { return path() == itm -> path() && refresh_path() == itm -> refresh_path(); }

        int activeSourceIndexLimit;
        Playlist * _parent;
        QList<IItem *> sources; // move later to fields as string list

        QString relationStr() const;
        QString errorStr(QString & key_name) const;
    };
}

#endif // ITEM_INTERFACE

