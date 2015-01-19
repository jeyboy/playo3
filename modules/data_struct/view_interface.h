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
#include "misc/stylesheets.h"

#include "tree_view_style.h"

//#include "model_item_delegate.h"

//#include "model_item.h"

//#include "media/library.h"
//#include "web/download.h"

namespace Playo3 {
    class ViewInterface : public QTreeView {
      Q_OBJECT
    public:
        ViewInterface(ModelInterface * model, QWidget * parent, ViewSettings & settins);
        ~ViewInterface();

        virtual QJsonObject toJson();

        void scrollToActive();

        void activatePrev();
        void activateNext();

        inline bool isRemoveFileWithItem() const { return sttngs.deleteFile; }
        inline bool isPlaylist() const { return sttngs.playlist; }
        inline bool isCommon() const { return sttngs.common; }
        inline bool isEditable() const { return sttngs.type < vk && !isCommon(); }


        QModelIndex fromPath(QString path);

        //template<class T> T * View::getModel() const {
        //    return dynamic_cast<T *>(model);
        //}

        inline ModelInterface * model() const { return mdl; }

        inline ViewSettings settings() const { return sttngs; }
        inline void setSettings(ViewSettings newSettings) { sttngs = newSettings; }

        inline QModelIndexList selectedItems() const { return selectedIndexes(); }

        void nextItem(bool deleteCurrent = false);
        void prevItem(bool deleteCurrent = false);
        bool execIndex(const QModelIndex & index);
//        virtual void removeItem(QModelIndex & index);

//        inline int itemsCount() const { return mdl -> itemsCount(); }

        void downloadSelected(QString savePath, bool markAsLiked = false);
        void copyItemsFrom(ViewInterface * otherView);
    public slots:
        inline void shuffle() { mdl -> shuffle(); }
        void updateSelection(QModelIndex & candidate);
//        void setHeaderText(QString);
//        void showMessage(QString);

    protected slots:
        inline void onDoubleClick(const QModelIndex & index) { execIndex(index); }
        void showContextMenu(const QPoint &);
        void openLocation();

        void download();
        void downloadAll();
//        void modelUpdate();

    protected:
//        ItemInterface * removeCandidate(ItemInterface * item);
        void drawRow(QPainter * painter, const QStyleOptionViewItem & options, const QModelIndex & index) const;
        void resizeEvent(QResizeEvent *);
        bool prepareDownloading(QString path);

        void downloadItem(const QModelIndex & item, QString savePath);
        void downloadBranch(const QModelIndex & node, QString savePath);


        QModelIndex activeItem(bool next = true);
        void toNextItem(QModelIndex & curr);
        void toPrevItem(QModelIndex & curr);

        void dragEnterEvent(QDragEnterEvent * event);
        void dragMoveEvent(QDragMoveEvent * event);
        virtual void dropEvent(QDropEvent  * event);

        void keyPressEvent(QKeyEvent * event);
        void mousePressEvent(QMouseEvent * event);
        void mouseMoveEvent(QMouseEvent * event);

        ModelInterface * mdl;
        ViewSettings sttngs;
        QPoint dragPoint;
    };
}

#endif // VIEW_H
