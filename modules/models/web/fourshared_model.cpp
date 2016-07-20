#include "fourshared_model.h"

using namespace Models;
/////////////////////////////////////////////////////////////

void FoursharedModel::refresh() {
    emit moveInProcess();
    QApplication::processEvents();
    Fourshared::Queries::obj().userInfo(
        sttngs.uid,
        new Func(this, SLOT(proceedJson(QJsonArray &)))
    );
}

void FoursharedModel::proceedJson(QJsonArray & hash) {
    int added = proceedList(dt_site_fourshared, hash, rootItem);

    if (added > 0) {
        int row_index = rootItem -> childCount();
        rootItem -> updateItemsCountInBranch(added);

        beginInsertRows(QModelIndex(), row_index, row_index + added);
        endInsertRows();
    }

    emit moveOutProcess();
}
