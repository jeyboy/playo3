//#ifndef SOUNDCLOUD_MODEL_H
//#define SOUNDCLOUD_MODEL_H

//#include "web/socials/soundcloud_api.h"
//#include "model/web/web_model.h"
//#include "media/media_player/utils/duration.h"

//class SoundcloudModel : public WebModel {
//    Q_OBJECT

//public:
//    SoundcloudModel(QString uid, QJsonObject * hash = 0, QObject *parent = 0);
//    ~SoundcloudModel();

//inline ContainerType containerType() const { return list; }
//    WebApi * getApi() { return SoundcloudApi::instance(); }
//public slots:
//    void refresh();

//protected slots:
//    void proceedResponse(QJsonObject &);
//    void proceedResponse(QJsonArray &, ModelItem *, QHash<ModelItem*, QString> &);

//protected:
//    QModelIndex dropProcession(const QList<QUrl> & list);

//    void filesRoutine(ModelItem * index, QFileInfo currFile);
//    void filesRoutine(ModelItem * index, QList<QUrl> list);
//};

//#endif // SOUNDCLOUD_MODEL_H
