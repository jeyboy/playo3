#ifndef DOWNLOAD_VIEW
#define DOWNLOAD_VIEW

#include <qdebug.h>

#include <qlistview.h>
#include <qevent.h>
#include <qmenu.h>
#include <qdrag.h>

#include <QtConcurrent/QtConcurrent>
#include <qfuturewatcher.h>

#include "download_model.h"
#include "download_delegate.h"

#include "misc/settings.h"
#include "misc/file_utils/filename_conversions.h"
//#include "misc/web_utils/custom_network_access_manager.h"
#include "modules/data_struct/item_drop_formats.h"
#include "modules/data_struct/model_item_parts/item_types.h"

#include "modules/web/socials_api/vk_api.h"

namespace Playo3 {
    class Dockbars;

    class DownloadView : public QListView {
      Q_OBJECT
    public:
        static DownloadView * instance(QJsonObject * hash = 0, QWidget * parent = 0);

        ~DownloadView();

        QJsonObject toJson();

        void scrollToActive();

        bool proceedDownload(QModelIndex & ind);
        void proceedDrop(QDropEvent * event, QString path);
    signals:
        void updateRequired(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles = QVector<int>());
        void updateAttr(const QModelIndex, int attr, QVariant val);
        void downloadProceeded(QString to);

    public slots:
        void onUpdateAttr(const QModelIndex, int attr, QVariant val);
        void downloadCompleted();

        void addRow(QUrl from, QString to, QString name, QString dtype = QString(), QString uid = QString());
        bool removeRow(const QModelIndex & node);

    protected slots:
        void reproceedDownload();
        void proceedDownload();

    protected:
        QString ioError(QFile * file);
        QString ioError(QNetworkReply * file);
        QModelIndex downloading(QModelIndex &, QFutureWatcher<QModelIndex> * watcher);

        void contextMenuEvent(QContextMenuEvent *);
        void removeProccessing(QModelIndexList & index_list);

        void dragEnterEvent(QDragEnterEvent *);
        void dragMoveEvent(QDragMoveEvent *);
        void dropEvent(QDropEvent *);

        void keyPressEvent(QKeyEvent *);
        void mousePressEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);

        DownloadModel * mdl;
        QPoint dragPoint;
    private:
        bool paused;

        DownloadView(QJsonObject * hash, QWidget * parent);
        QList<QFutureWatcher<QModelIndex> *> watchers;
        QHash<QModelIndex, QFutureWatcher<QModelIndex> *> bussyWatchers;

        static DownloadView * self;

        DownloadDelegate * item_delegate;
    };
}
#endif // DOWNLOAD_VIEW
