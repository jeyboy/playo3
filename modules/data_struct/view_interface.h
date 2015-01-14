#ifndef VIEW_H
#define VIEW_H

#include <QMimeData>
#include <QTreeView>
#include <QMouseEvent>
#include <QPixmap>
#include <QFileInfo>
#include <QJsonObject>
#include <QDrag>
#include <QHeaderView>
#include <QMenu>
#include <QMessageBox>
#include <QList>

#include "model_interface.h"
#include "dialogs/tabdialog.h"

#include "view_settings.h"
#include "misc/settings.h"

#include "tree_view_style.h"

//#include "model_item_delegate.h"

//#include "model_item.h"

//#include "media/library.h"
//#include "web/download.h"

namespace Playo3 {
    class ViewInterface : public QTreeView {
      Q_OBJECT
    public:
        ViewInterface(ModelInterface * model, QWidget * parent, ViewSettings settins);
        ~ViewInterface();

        virtual QJsonObject toJSON();

        void scrollToActive();

        void activatePrev();
        void activateNext();

        inline bool isRemoveFileWithItem() const { return sttgs.deleteFile; }
        inline bool isPlaylist() const { return sttgs.playlist; }
        inline bool isCommon() const { return sttgs.common; }
        inline bool isEditable() const { return sttgs.type < 4 && !isCommon(); }


        ModelInterface * fromPath(QString path);

        //template<class T> T * View::getModel() const {
        //    return dynamic_cast<T *>(model);
        //}

        inline ModelInterface * model() const { return mdl; }

        inline ViewSettings getSettings() const { return sttgs; }
        inline void setSettings(ViewSettings newSettings) { sttgs = newSettings; }

        inline QModelIndexList selectedItems() const { return selectedIndexes(); }

        bool execItem(ModelItem * item, bool paused = false);
        virtual void removeItem(ModelItem * item);

        inline int itemsCount() const { return mdl -> itemsCount(); }

        void downloadSelected(QString savePath, bool markAsLiked = false);
        void copyItemsFrom(ViewInterface * otherView);
    public slots:
        void shuffle();
        void updateSelection(QModelIndex candidate);
        void setHeaderText(QString);
        void showMessage(QString);

    protected slots:
        void onDoubleClick(const QModelIndex &index);
        void showContextMenu(const QPoint &);
        void openLocation();

        void download();
        void downloadAll();
        void modelUpdate();

    protected:
        ModelItem * removeCandidate(ModelItem * item);
        void drawRow(QPainter * painter, const QStyleOptionViewItem & options, const QModelIndex &index) const;
        void resizeEvent(QResizeEvent *);
        bool prepareDownloading(QString path);

        void downloadItem(ModelItem * item, QString savePath);
        void downloadBranch(ModelItem * rootNode, QString savePath);


        QModelIndex * activeItem(bool next = true);
        QModelIndex * nextItem(QModelIndex * curr);
        QModelIndex * prevItem(QModelIndex * curr);

        void dragEnterEvent(QDragEnterEvent * event);
        void dragMoveEvent(QDragMoveEvent * event);
        virtual void dropEvent(QDropEvent  *event);

        void keyPressEvent(QKeyEvent * event);
        void mousePressEvent(QMouseEvent * event);
        void mouseMoveEvent(QMouseEvent * event);

        ModelInterface * mdl;
        ViewSettings sttgs;
        QPoint dragPoint;
    };
}

#endif // VIEW_H
