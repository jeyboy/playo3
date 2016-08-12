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
    proceedBlocks(dt_site_fourshared, hash, rootItem);

    emit moveOutProcess();
}
