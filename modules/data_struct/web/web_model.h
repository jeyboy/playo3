#ifndef WEB_MODEL_H
#define WEB_MODEL_H

#include "modules/data_struct/model_interface.h"
#include "deleted_list.h"

class WebModel : public IModel, public DeletedList {
    Q_OBJECT

public:
    WebModel(QString uid, QJsonObject * hash = 0, QObject * parent = 0);
    ~WebModel();

    QString getTabUid() const;
protected slots:
    void errorReceived(int, QString);
protected:
    void deleteRemoved(QHash<ModelItem*, QString> & store);
    QString tabUid;
};


#endif // WEB_MODEL_H
