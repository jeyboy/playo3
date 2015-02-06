#ifndef VIEW_H
#define VIEW_H

#include <QTreeView>
#include <QHeaderView>
#include <QMenu>
#include <QDrag>
#include <QMessageBox>

#include <qevent.h>

#include "model_interface.h"
#include "dialogs/tabdialog.h"

#include "view_settings.h"
#include "misc/settings.h"
#include "misc/stylesheets.h"

#include "tree_view_style.h"

#include "media/player.h"

#include "model_item_delegate.h"

//#include "media/library.h"
//#include "web/download.h"

namespace Playo3 {
    class Dockbars;

    class ViewInterface : public QTreeView {
      Q_OBJECT
    public:
        ViewInterface(ModelInterface * model, QWidget * parent, ViewSettings & settins);
        ~ViewInterface();

        inline virtual QJsonObject toJson() { return mdl -> toJson(); }

        void scrollToActive();

        void activatePrev();
        void activateNext();

        inline bool isRemoveFileWithItem() const { return sttngs.deleteFile; }
        inline bool isPlaylist() const { return sttngs.playlist; }
        inline bool isCommon() const { return sttngs.common; }
        inline bool isEditable() const { return sttngs.type < vk && !isCommon(); }

//        inline ModelInterface * model() const { return mdl; }

        inline ViewSettings settings() const { return sttngs; }
        inline void setSettings(ViewSettings newSettings) { sttngs = newSettings; }

        inline QModelIndexList selectedItems() const { return selectedIndexes(); }

        void execNextIndex(bool deleteCurrent = false);
        void execPrevIndex(bool deleteCurrent = false);
        bool execIndex(const QModelIndex & node);
//        virtual void removeItem(QModelIndex & node);

//        inline int itemsCount() const { return mdl -> itemsCount(); }

        void downloadSelected(QString savePath, bool markAsLiked = false);

        void setIconSize(const QSize & size);
    public slots:
        void expandeAll();
        void collapseAll();
        inline void shuffle() { mdl -> shuffle(); }
        void onSpoilNeeded(const QModelIndex &);
        void updateSelection(QModelIndex &);
//        void setHeaderText(QString);
//        void showMessage(QString);

    protected slots:
        inline void itemNotExist(QModelIndex & node) { mdl -> setData(node, ItemState::not_exist);}
        inline void itemNotSupported(QModelIndex & node) { mdl -> setData(node, ItemState::not_supported);}
        inline void itemError(QModelIndex & /*node*/) { /*TODO: reaction needed*/ }
        inline void onDoubleClick(const QModelIndex & node) { execIndex(node); }
        void openLocation();

        void download();
        void downloadAll();
//        void modelUpdate();

    protected:
        QModelIndex candidateOnSelection(QModelIndex node);
        void findAndExecIndex(bool deleteCurrent);
        bool removeRow(QModelIndex & node, bool updateSelection = false);
        bool removeRows(QModelIndexList & nodes, bool updateSelection = false);

        bool prepareDownloading(QString path);

        void downloadItem(const QModelIndex & node, QString savePath);
        void downloadBranch(const QModelIndex & node, QString savePath);

        QModelIndex activeIndex();
        void findExecutable(QModelIndex &);

//        ItemInterface * removeCandidate(ItemInterface * item);
        void drawRow(QPainter * painter, const QStyleOptionViewItem & options, const QModelIndex & node) const;
        void resizeEvent(QResizeEvent *);
        void focusInEvent(QFocusEvent *);
        void contextMenuEvent(QContextMenuEvent *);

        void dragEnterEvent(QDragEnterEvent *);
        void dragMoveEvent(QDragMoveEvent *);
        virtual void dropEvent(QDropEvent  *);

        void keyPressEvent(QKeyEvent *);
        void mousePressEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);

        ModelInterface * mdl;
        ViewSettings sttngs;
        QPoint dragPoint;
        bool forwardOrder;
    private:
        ModelItemDelegate * item_delegate;

    };
}

#endif // VIEW_H
