#ifndef DOWNLOAD_VIEW
#define DOWNLOAD_VIEW

#include <qlistview.h>
#include <qevent.h>
#include <qmenu.h>
#include <qdrag.h>

#include "download_model.h"
#include "download_delegate.h"

namespace Playo3 {
    class Dockbars;

    class DownloadView : public QListView {
      Q_OBJECT
    public:
        static DownloadView * instance(QWidget * parent = 0);

        ~DownloadView();

//        inline virtual QJsonObject toJson() { return mdl -> toJson(); }

        void scrollToActive();

    signals:

    public slots:
        void openLocation();
        void addRow(const QVector<QVariant> & data);
        bool removeRow(const QModelIndex & node);
    protected:
        void contextMenuEvent(QContextMenuEvent *);
        void removeProccessing(QModelIndexList & index_list, bool inProcess = false);

        void dragEnterEvent(QDragEnterEvent *);
        void dragMoveEvent(QDragMoveEvent *);
        virtual void dropEvent(QDropEvent  *);

        void keyPressEvent(QKeyEvent *);
        void mousePressEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);

        DownloadModel * mdl;
        QPoint dragPoint;
    private:
        DownloadView(QWidget * parent);

        static DownloadView * self;

//        DownloadDelegate * item_delegate;
    };
}

#endif // DOWNLOAD_VIEW
