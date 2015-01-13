#include "soundcloud_view.h"
#include <QDebug>

SoundcloudView::SoundcloudView(QWidget *parent, CBHash settingsSet, QJsonObject *hash)
    : TreeView(dynamic_cast<Model *>(new SoundcloudModel(QString::number(settingsSet.value("uid")), hash)), parent, settingsSet) {
}

SoundcloudView::~SoundcloudView() {}

QJsonObject SoundcloudView::toJSON() {
    QJsonObject res = TreeView::toJSON();

    res.insert("uid", ((SoundcloudModel *)model) -> getTabUid());

    return res;
}
