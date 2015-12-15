#ifndef DOWNLOAD_VIEW
#define DOWNLOAD_VIEW

#include <qlistview.h>
#include <qevent.h>
#include <qmenu.h>
#include <qdrag.h>

#include <QtConcurrent/QtConcurrent>
#include <qfuturewatcher.h>

#include "modules/models/service/download_model.h"
#include "modules/models/service/download_delegate.h"

#include "settings.h"
#include "modules/core/misc/file_utils/filename_conversions.h"
#include "modules/core/core_parts/item_drop_formats.h"
#include "modules/core/core_parts/core_part_types.h"
#include "modules/core/core_parts/part_mixes/item_fields.h"

#include "modules/core/web/apis/social/vk_api.h"
#include "modules/core/interfaces/singleton.h"
#include "modules/core/misc/file_utils/file_errors.h"
#include "modules/core/web/response_error.h"

// TODO: rewrite on mutex using while deleting items
namespace Views {
    class DownloadView : public QListView, public Core::SingletonPtr<DownloadView>, public Core::FileErrors, public Core::ResponseError {
      Q_OBJECT
    public:
        ~DownloadView();

        QJsonObject toJson();

        inline void scrollToActive() { scrollTo(currentIndex()); }

        bool proceedDownload(QModelIndex & ind);
        void proceedDrop(QDropEvent * event, QString path);
    signals:
        void updateRequired(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles = QVector<int>());
        void updateAttr(const QModelIndex, int attr, QVariant val);
        void downloadProceeded(QString to);

    public slots:
        void onUpdateAttr(const QModelIndex, int attr, QVariant val);
        void downloadCompleted();

        void addRow(QUrl from, QString to, QString name, QString dtype = QString(), QString uid = QString());
        bool removeRow(const QModelIndex & node);

    protected slots:
        void downloadRemoteProgress(qint64 bytesReceived, qint64 bytesTotal);
        void reproceedDownload();
        void proceedDownload();

    protected:
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
        QHash<QIODevice *, QModelIndex> downIndexes;
        bool paused;

        friend class Core::SingletonPtr<DownloadView>;
        DownloadView(QJsonObject * hash, QWidget * parent);
        DownloadView() {}

        QList<QFutureWatcher<QModelIndex> *> watchers;
        QHash<QModelIndex, QFutureWatcher<QModelIndex> *> bussyWatchers;

        DownloadDelegate * item_delegate;
    };
}
#endif // DOWNLOAD_VIEW
