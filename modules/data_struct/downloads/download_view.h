#ifndef DOWNLOAD_VIEW
#define DOWNLOAD_VIEW

#include <qprogressbar.h>
#include <qlistview.h>
#include <qevent.h>
#include <qmenu.h>
#include <qdrag.h>

#include <QtConcurrent/QtConcurrent>
#include <qfuturewatcher.h>

#include "download_model.h"
#include "download_delegate.h"

#include "misc/web_utils/custom_network_access_manager.h"

//void update(const QModelIndex &index);

namespace Playo3 {
    class Dockbars;

    class DownloadView : public QListView {
      Q_OBJECT
    public:
        static DownloadView * instance(QJsonObject * hash = 0, QWidget * parent = 0);

        ~DownloadView();

        inline QJsonObject toJson() { return mdl -> toJson(); }

        void scrollToActive();

        void proceedDownload(QModelIndex & ind);
    signals:
        void updateRequired(const QModelIndex & index);

    public slots:
        void downloadCompleted();

        void addRow(QUrl from, QString to, QString name);
        bool removeRow(const QModelIndex & node);

    protected:
        QModelIndex downloading(QModelIndex &);

        void contextMenuEvent(QContextMenuEvent *);
        void removeProccessing(QModelIndexList & index_list);

        void dragEnterEvent(QDragEnterEvent *);
        void dragMoveEvent(QDragMoveEvent *);
        virtual void dropEvent(QDropEvent  *);

        void keyPressEvent(QKeyEvent *);
        void mousePressEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);

        DownloadModel * mdl;
        QPoint dragPoint;
    private:
        DownloadView(QJsonObject * hash, QWidget * parent);
        QList<QFutureWatcher<QModelIndex> *> watchers;
        QList<QFutureWatcher<QModelIndex> *> bussyWatchers;

        static DownloadView * self;

        DownloadDelegate * item_delegate;
        CustomNetworkAccessManager * networkManager;
    };
}

#endif // DOWNLOAD_VIEW