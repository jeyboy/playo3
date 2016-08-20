#include "od_model.h"

using namespace Models;
/////////////////////////////////////////////////////////////

void OdModel::refresh() {
    emit moveInProcess();

    Od::Queries::obj().userInfoAsync(
        sttngs.uid,
        new Func(this, SLOT(proceedJson(QJsonValue &)))
    );
}

void OdModel::proceedJson(QJsonValue & jval) {
    proceedBlocks(jval.toArray(), rootItem);
    emit moveOutProcess();
}
