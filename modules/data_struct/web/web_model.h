#ifndef WEB_MODEL_H
#define WEB_MODEL_H

#include "model/model.h"
#include "model/tree/tree_model.h"
#include "model/web/deleted_list.h"

class WebModel : public TreeModel, public DeletedList {
    Q_OBJECT

public:
    WebModel(QString uid, QJsonObject * hash = 0, QObject *parent = 0);
    ~WebModel();

    QString getTabUid() const;
protected slots:
    void errorReceived(int, QString);
protected:
    void deleteRemoved(QHash<ModelItem*, QString> & store);
    QString tabUid;
};


#endif // WEB_MODEL_H
