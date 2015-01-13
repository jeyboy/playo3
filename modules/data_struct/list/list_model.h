#ifndef LIST_MODEL_H
#define LIST_MODEL_H

#include "model/model.h"

class ListModel : public Model {
    Q_OBJECT

public:
    ListModel(QJsonObject * hash = 0, QObject *parent = 0);
    ~ListModel();
protected:
    QModelIndex dropProcession(const QList<QUrl> & list);
    void filesRoutine(ModelItem * index, QFileInfo currFile);
    void filesRoutine(ModelItem * index, QList<QUrl> list);
};

#endif // LIST_MODEL_H
