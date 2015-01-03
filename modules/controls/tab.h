#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QBoxLayout>
#include <QJsonObject>

//#include "views/index.h"
//#include "tab_settings.h"
//#include "../models/model_interface.h"

//#ifndef CBHASH
//#define CBHASH

//  #include <QHash>
//  typedef QHash <QString, int> CBHash;
//  Q_DECLARE_METATYPE(CBHash);
//#endif // CBHASH

    class Tab : public QWidget {
        Q_OBJECT
    //public:
    //    void init(TabSettings params, QJsonObject * hash = 0);

    //    explicit Tab(TabSettings params, QWidget * parent = 0);
    //    explicit Tab(QJsonObject hash, QWidget * parent = 0);
    //    ~Tab();

    //    QString getName() const;
    //    void setName(QString newName);

    ////    View * getView() const;
    //    QJsonObject toJSON(QString name);

    //    bool isEditable() const;

    //public slots:
    //    void updateHeader(int new_count = -1);

    ////protected slots:
    ////    void startRoutine();
    ////    void stopRoutine();

    //protected:
    //    void setNameWithCount(QString name);

    //private:
    //    View * view;
    //    QTabWidget * tabber;

    //    QLabel * spinnerContainer;
    };

#endif // TAB_H


//MyItemModel *sourceModel = new MyItemModel(this);
//QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel(this);

//proxyModel->setSourceModel(sourceModel);
//treeView->setModel(proxyModel);



//treeView->setSortingEnabled(true);



//proxyModel->sort(2, Qt::AscendingOrder);

//proxyModel->setFilterRegExp(QRegExp(".png", Qt::CaseInsensitive,
//                                    QRegExp::FixedString));
//proxyModel->setFilterKeyColumn(1);
