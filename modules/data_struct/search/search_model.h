#ifndef SEARCH_MODEL_H
#define SEARCH_MODEL_H

#include "../model_interface.h"
#include "search_settings.h"

namespace Playo3 {
    class SearchModel : public IModel {
        Q_OBJECT

    public:
        SearchModel(QJsonObject * hash = 0, QObject * parent = 0);
        ~SearchModel();

        inline bool isRelative() const { return false; }
        inline ContainerType containerType() const { return search; }

        void initiateSearch(SearchSettings params);
    protected:
        void recalcParentIndex(const QModelIndex & dIndex, int & dRow, QModelIndex & exIndex, int & exRow, QUrl url);
        void dropProcession(const QModelIndex & ind, int row, const QList<QUrl> & list);
        int filesRoutine(QFileInfo & currFile, FolderItem * node, QHash<FolderItem *, int> & rels);
        int filesRoutine(const QList<QUrl> & list, FolderItem * node, int pos = -1);
    protected slots:
        void proceedVk(QJsonObject & collection);
        void proceedTabs(SearchSettings params);
    };
}

#endif // SEARCH_MODEL_H
