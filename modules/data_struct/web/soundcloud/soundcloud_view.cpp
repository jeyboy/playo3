#include "soundcloud_view.h"

using namespace Playo3;

SoundcloudView::SoundcloudView(QWidget * parent, ViewSettings settings, QJsonObject * hash)
    : IView(dynamic_cast<IModel *>(new SoundcloudModel(settings.uid, hash)), parent, settings) {}

SoundcloudView::SoundcloudView(WebModel * newModel, QWidget * parent, ViewSettings settings) : IView(newModel, parent, settings) {}

SoundcloudView::~SoundcloudView() {}
