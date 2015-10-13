#ifndef VIEW_H
#define VIEW_H

#include <qtreeview.h>
#include <qheaderview.h>
#include <qmenu.h>
#include <qdrag.h>
#include <qevent.h>

#include "dialogs/tabdialog.h"
#include "dialogs/user_action_dialog.h"

#include "view_settings.h"
#include "settings.h"
#include "stylesheets.h"

#include "tree_view_style.h"

#include "modules/core/data_core.h"
#include "modules/models/model_item_delegate.h"
#include "modules/models/model_interface.h"
#include "service/download_view.h"

//qDebug() << this->table->rowAt( 0 ) << "-" << this->table->rowAt( this->table->height() ); // this is what you want
//qDebug() << this->table->columnAt( 0 ) << "-" << this->table->columnAt( this->table->width() ); // this is what you want

namespace View {
    using namespace Models;

    class IView : public QTreeView {
      Q_OBJECT
    public:
        void registerActions() { emit registerSync(mdl, mdl -> syncMutex()); }
        void registerParent(QWidget * newParent);
        IView(IModel * model, QWidget * parent, Params & settins);
        virtual ~IView();

        inline QJsonObject toJson() { return mdl -> toJson(); }

        void scrollToActive();

        void activatePrev();
        void activateNext();

        inline bool isRemoveFileWithItem() const { return sttngs.deleteFile; }
        inline bool isPlaylist() const { return sttngs.playlist; }
        inline bool isCommon() const { return sttngs.common; }
        inline bool isEditable() const { return sttngs.type < Data::vk && !isCommon(); }
        inline bool isRequiredOnUpdate() const { return sttngs.type == Data::vk; }

        inline Params settings() const { return sttngs; }
        inline void setSettings(Params newSettings) { sttngs = newSettings; }

        void execNextIndex(bool deleteCurrent = false);
        void execPrevIndex(bool deleteCurrent = false);
        bool execIndex(const QModelIndex & node, bool paused = false, uint start = 0, int duration = -1);
        bool execPath(const QString path, bool paused = false, uint start = 0, int duration = -1);

//        inline int itemsCount() const { return mdl -> itemsCount(); }

        void setIconSize(const QSize & size);
        void appendRows(QList<QUrl> & urls);
        void markSelectedAsLiked(bool liked);
    signals:
        void registerSync(QAbstractItemModel * mdl, QMutex * mutex);
        void unregisterSync(QAbstractItemModel * mdl);
        void discardSync(QAbstractItemModel * mdl);
        void changeCadrSize(QAbstractItemModel * mdl, int newSize);
        void infoInvalidation(const QModelIndex & node) const;
        void infoInvalidationAsync(const QModelIndex & node) const;

        void showAlert(const QString & title, const QString & text, QMessageBox::StandardButtons buttons);
        void threadedRowRemoving(const QModelIndex & node, bool remove, int selectionUpdate, bool usePrevAction);
        void searchFinished();

    public slots:
        bool startInnerSearch(QString, QModelIndex it = QModelIndex());
        void onUpdateAttr(const QModelIndex, int attr, QVariant val);

        void expandeAll();
        void collapseAll();
        inline void shuffle() { mdl -> shuffle(); }
        void onSpoilNeeded(const QModelIndex &);
        void updateSelection(QModelIndex &);
//        void setHeaderText(QString);
//        void showMessage(QString);

    protected slots:
        inline void onDoubleClick(const QModelIndex node) {
            if (!execIndex(node) && !node.data(IFOLDER).toBool()) { // find first valid for exec
                if (::Settings::instance() -> isCheckboxShow()) {
                    QModelIndex node = QModelIndex();
                    findExecutable(node);
                    execIndex(node);
                }
            }
        }
        void openLocation();
        void copyToClipboard();
        void copyIdsToClipboard();
        void importIds();
        void openRecomendationsforUser();
        void openRecomendationsforItem();
        void openRecomendationsforItemUser();
        bool removeRow(const QModelIndex & node, bool remove_file_with_item, int selectionUpdate = IModel::none, bool usePrevAction = false);

        void downloadSelected();
        void downloadChecked(QString & path, Playlist * root = 0);
        void downloadAll();

        void markLikedAsChecked();
        void markNewAsChecked();
        void markListenedAsChecked();
        void moveCheckedToNewTab(Playlist * root = 0);

    protected:
        void checkByPredicate(IItem::ItemStateFlag flag);
        QModelIndex candidateOnSelection(QModelIndex node, bool reverseOrder = false);
        void findAndExecIndex(bool deleteCurrent);
        void removeProccessing(QModelIndexList & index_list, bool remove, bool inProcess = false);
        void removeSelectedItems(bool remove = true);

        void downloadItems(const QModelIndexList & nodes, QString savePath);
        void downloadBranch(const QModelIndex & node, QString savePath);

        QModelIndex activeIndex();
        void findExecutable(QModelIndex &);

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
        Params sttngs;
        QPoint dragPoint;
        IModel::Direction direction;
        int _deleteFolderAnswer;
    private:
        ModelItemDelegate * item_delegate;
        bool blockRepaint;
    };
}

#endif // VIEW_H
