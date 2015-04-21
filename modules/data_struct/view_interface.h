#ifndef VIEW_H
#define VIEW_H

#include <QTreeView>
#include <QHeaderView>
#include <QMenu>
#include <QDrag>

#include <qevent.h>

#include "model_interface.h"
#include "dialogs/tabdialog.h"

#include "view_settings.h"
#include "misc/settings.h"
#include "misc/stylesheets.h"

#include "downloads/download_view.h"

#include "tree_view_style.h"

#include "modules/controls/user_dialog_box.h"

#include "model_item_delegate.h"

#include "media/player.h"

//qDebug() << this->table->rowAt( 0 ) << "-" << this->table->rowAt( this->table->height() ); // this is what you want
//qDebug() << this->table->columnAt( 0 ) << "-" << this->table->columnAt( this->table->width() ); // this is what you want

namespace Playo3 {
    class Dockbars;

    class IView : public QTreeView {
      Q_OBJECT
    public:
        enum SearchDirection {
            none = 0,
            forward = 1,
            backward = 2
        };

        IView(IModel * model, QWidget * parent, ViewSettings & settins);
        ~IView();

        inline QJsonObject toJson() { return mdl -> toJson(); }

        void scrollToActive();

        void activatePrev();
        void activateNext();

        inline bool isRemoveFileWithItem() const { return sttngs.deleteFile; }
        inline bool isPlaylist() const { return sttngs.playlist; }
        inline bool isCommon() const { return sttngs.common; }
        inline bool isEditable() const { return sttngs.type < vk && !isCommon(); }
        inline bool isRequiredOnUpdate() const { return sttngs.type == vk; }

//        inline IModel * model() const { return mdl; }

        inline ViewSettings settings() const { return sttngs; }
        inline void setSettings(ViewSettings newSettings) { sttngs = newSettings; }

        void execNextIndex(bool deleteCurrent = false);
        void execPrevIndex(bool deleteCurrent = false);
        bool execIndex(const QModelIndex & node, bool paused = false, uint start = 0);
        bool execPath(const QString path, bool paused = false, uint start = 0);

//        inline int itemsCount() const { return mdl -> itemsCount(); }

        void setIconSize(const QSize & size);
        void appendRows(QList<QUrl> & urls);

    signals:
        showAlert(const QString & title, const QString & text, QMessageBox::StandardButtons buttons);
        void threadedRowRemoving(QModelIndex node, int selectionUpdate, bool usePrevAction);

    public slots:
        void onUpdateAttr(const QModelIndex, int attr, QVariant val);

        void expandeAll();
        void collapseAll();
        inline void shuffle() { mdl -> shuffle(); }
        void onSpoilNeeded(const QModelIndex &);
        void updateSelection(QModelIndex &);
//        void setHeaderText(QString);
//        void showMessage(QString);

    protected slots:
        inline void itemNotExist(QModelIndex node) { mdl -> setData(node, ItemState::not_exist); }
        inline void itemNotSupported(QModelIndex node) { mdl -> setData(node, ItemState::not_supported); }
        inline void itemError(QModelIndex node) { mdl -> setData(node, ItemState::not_supported); }
        inline void onDoubleClick(const QModelIndex node) { execIndex(node); }
        void openLocation();
        void copyToClipboard();
        bool removeRow(const QModelIndex & node, int selectionUpdate = none, bool usePrevAction = false);

        void downloadSelected();
        void downloadAll();

    protected:
        QModelIndex candidateOnSelection(QModelIndex node, bool reverseOrder = false);
        void findAndExecIndex(bool deleteCurrent);
        void removeProccessing(QModelIndexList & index_list, bool inProcess = false);
        void removeSelectedItems();

        void downloadItem(const QModelIndex & node, QString savePath);
        void downloadBranch(const QModelIndex & node, QString savePath);

        QModelIndex activeIndex();
        void findExecutable(QModelIndex &);

//        IItem * removeCandidate(IItem * item);
        void drawRow(QPainter * painter, const QStyleOptionViewItem & options, const QModelIndex & node) const;
        void paintEvent(QPaintEvent * event);
        void resizeEvent(QResizeEvent *);
        void focusInEvent(QFocusEvent *);
        void contextMenuEvent(QContextMenuEvent *);

        void dragEnterEvent(QDragEnterEvent *);
        void dragMoveEvent(QDragMoveEvent *);
        virtual void dropEvent(QDropEvent  *);

        void keyPressEvent(QKeyEvent *);
        void mousePressEvent(QMouseEvent *);
        void mouseMoveEvent(QMouseEvent *);

        IModel * mdl;
        ViewSettings sttngs;
        QPoint dragPoint;
        bool forwardOrder;
        int _deleteFolderAnswer;
    private:
        ModelItemDelegate * item_delegate;
        bool blockRepaint;
    };
}

#endif // VIEW_H
