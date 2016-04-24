#ifndef VIEW_H
#define VIEW_H

#include <qtreeview.h>
#include <qheaderview.h>
#include <qmenu.h>
#include <qdrag.h>
#include <qevent.h>

#include "dialogs/tabdialog.h"
#include "dialogs/user_action_dialog.h"

#include "settings.h"

#include "tree_view_style.h"

#include "modules/core/data_factory.h"
#include "modules/models/model_item_delegate.h"
#include "modules/models/model_interface.h"
#include "service/download_view.h"

//qDebug() << this->table->rowAt( 0 ) << "-" << this->table->rowAt( this->table->height() ); // this is what you want
//qDebug() << this->table->columnAt( 0 ) << "-" << this->table->columnAt( this->table->width() ); // this is what you want

#define MODEL_POSTFIX QStringLiteral("_mdl")

namespace Views {
    using namespace Models;

    enum DelFlags : int {
        none = 0,
        use_prev_action = 1,
        dont_remove_played = 2,
    };

    class IView : public QTreeView, public IPlaylistable {
        Q_OBJECT

        TreeViewStyle * tree_style;
        ModelItemDelegate * item_delegate;
        bool blockRepaint;
        bool blockDeletion;
    public:
        inline void registerActions() { emit registerSync(mdl, mdl -> syncMutex()); }
        void registerParent(QWidget * newParent);
        IView(IModel * model, QWidget * parent);
        virtual ~IView();

        inline QJsonObject toJson() { return mdl -> toJson(); }

        void scrollToActive();

        bool restoreItem(IItem * itm) { return mdl -> restoreUrl(itm); }

        inline bool isRemoveFileWithItem() const { return settings().deleteFile; }
        inline bool isPlaylist() const { return settings().playlist; }
        inline bool isCommon() const { return settings().common; }
        inline bool isEditable() const { return settings().type < Data::vk && !isCommon(); }
        inline bool isSearch() const { return settings().type == Data::search; }

        inline Params settings() const { return mdl -> settings(); }
        inline void setSettings(const Params & newSettings) { mdl -> setSettings(newSettings); }

        void execNextIndex(bool deleteCurrent = false);
        void execPrevIndex(bool deleteCurrent = false);
        bool execIndex(const QModelIndex & node, PlayerInitState init_state = played, uint start = 0);
        bool execPath(const QString & path, PlayerInitState init_state = played, uint start = 0);
        bool restoreSelection(const QString & path);

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
        void infoInvalidationAsync(const QModelIndex & node, const bool & isRemote) const;

        void showAlert(QWidget * parent, const QString & title, const QString & text, QMessageBox::StandardButtons buttons);
        void threadedRowRemoving(const QModelIndex & node, bool remove, int selectionUpdate, int flags);
        void searchFinished();

    public slots:
        bool startInnerSearch(QString, QModelIndex it = QModelIndex());
        void onUpdateAttr(const QModelIndex &, int attr, const QVariant & val);

        void expandeAll();
        void collapseAll();
        inline void shuffle() { mdl -> shuffle(); }
        void onSpoilNeeded(const QModelIndex &);
        void updateSelection(QModelIndex &);
//        void setHeaderText(QString);
//        void showMessage(QString);

    protected slots:
        void updateRemovingBlockation(bool isBlocked) { blockDeletion = isBlocked; }
        inline void onDoubleClick(const QModelIndex node) {
            if (!execIndex(node) && !node.data(IFOLDER).toBool()) { // find first valid for exec
                if (Settings::obj().isCheckboxShow()) {
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
        bool removeRow(const QModelIndex & node, bool remove_file_with_item, int selectionUpdate = IModel::none, int flags = none);

        void downloadSelected();
        void downloadChecked(QString & path, Playlist * root = 0);
        void downloadAll();

        void markLikedAsChecked();
        void markNewAsChecked();
        void markListenedAsChecked();
        void moveCheckedToNewTab(Playlist * root = 0);

    protected:
        bool setData(QModelIndex ind, const QVariant & value, int role = ISTATE) { return mdl -> setData(ind, value, role); }
        QModelIndex index(IItem * item) const { return mdl -> index(item); }

        void checkByPredicate(IItem::ItemStateFlag flag);
        QModelIndex candidateOnSelection(QModelIndex node, bool reverseOrder = false);
        void findAndExecIndex(bool deleteCurrent);
        void removeProccessing(QModelIndexList & index_list, bool remove, int flags = none);
        void removeSelectedItems(bool remove = true, int flags = none);

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
        QPoint dragPoint;
        IModel::Direction direction;
        int _deleteFolderAnswer;
    };
}

#endif // VIEW_H
