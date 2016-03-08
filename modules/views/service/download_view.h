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
#include "modules/core/core_parts/part_mixes/item_fields.h"

#include "modules/core/web/apis/social/vk_api.h"
#include "modules/core/interfaces/singleton.h"
#include "modules/core/misc/file_utils/file_errors.h"
#include "modules/core/web/response_error.h"

namespace Views {
    class DownloadView : public QListView, public Core::SingletonPtr<DownloadView>, public Core::FileErrors, public Core::ResponseError {
      Q_OBJECT

      bool toggleNeeded;
    public:
        ~DownloadView();

        QJsonObject toJson();

        inline void scrollToActive() { scrollTo(currentIndex()); }

        bool proceedDownload(DownloadModelItem * item);
        void proceedDrop(QDropEvent * event, const QString & path);
        bool initiateDownloading(DownloadModelItem * item);
    signals:
        void updateRequired(const QModelIndex & topLeft, const QModelIndex & bottomRight, const QVector<int> & roles = QVector<int>());
        void updateAttr(DownloadModelItem * item, int attr, QVariant val);
        void downloadProceeded(QString to);

    public slots:
        void onUpdateAttr(DownloadModelItem * item, int attr, const QVariant & val) { mdl -> setData(mdl -> index(item), val, attr); }
        void initiateSaving(DownloadModelItem * item, QIODevice * source);
        void savingCompleted();

        void addRow(const QUrl & from, const QString & to, const QString & name, int dtype = 0, const QString & refresh_attrs = QString());
        bool removeRow(DownloadModelItem * item);

    protected slots:
        void asyncRequestFinished(QIODevice * source, void * userData);
        void downloadRemoteProgress(qint64 bytesReceived, qint64 bytesTotal);
        void reproceedDownload();
        void proceedDownload();

    protected:
        void initiateItem(DownloadModelItem * itm);
        void setItemError(DownloadModelItem * itm, const QString & err = QStringLiteral("unprocessable"));
        QUrl restoreUrl(DownloadModelItem * itm);
        DownloadModelItem * saving(DownloadModelItem * itm, QIODevice * source, QFutureWatcher<DownloadModelItem *> * watcher);

        void contextMenuEvent(QContextMenuEvent *);

        void dragEnterEvent(QDragEnterEvent *);
        void dragMoveEvent(QDragMoveEvent *);
        void dropEvent(QDropEvent *);

        void keyPressEvent(QKeyEvent *);
        void mousePressEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);

        DownloadModel * mdl;
        QPoint dragPoint;
    private:
        QHash<QIODevice *, DownloadModelItem *> downIndexes;
        bool paused;

        friend class Core::SingletonPtr<DownloadView>;
        DownloadView(QJsonObject * hash, QWidget * parent);
        DownloadView() {}

        QHash<DownloadModelItem *, QFutureWatcher<DownloadModelItem *> *> bussyWatchers;
    };
}
#endif // DOWNLOAD_VIEW
