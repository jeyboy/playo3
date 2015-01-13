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

//#include "model_item_delegate.h"

//#include "model_item.h"

namespace Playo3 {
    class ViewInterface : public QTreeView {
      Q_OBJECT
    public:
        ViewInterface(ModelInterface * newModel, QWidget * parent, ViewSettings settins);
        ~ViewInterface();

        virtual QJsonObject toJSON();

        void scrollToActive();

        void proceedPrev();
        void proceedNext();
        void deleteCurrentProceedNext();

        inline bool isRemoveFileWithItem() const { return sttgs.deleteFile; }
        inline bool isPlaylist() const { return sttgs.playlist; }
        inline bool isCommon() const { return sttgs.common; }
        inline bool isEditable() const { return sttgs.type < 4 && !isCommon(); }


        ModelInterface * fromPath(QString path);

    //    template<class T> T * getModel() const;
        inline ModelInterface * model() const { return mdl; }

        inline ViewSettings getSettings() const { return sttgs; }
        inline void setSettings(ViewSettings newSettings) { sttgs = newSettings; }

        QModelIndexList selectedItems() const;

        bool execItem(ModelItem * item, bool paused = false);
        virtual void removeItem(ModelItem * item);

        int itemsCount() const;

        void downloadSelected(QString savePath, bool markAsLiked = false);
        void copyItemsFrom(View * otherView);

    signals:
        void showSpinner();
        void hideSpinner();

    public slots:
        void shuffle();
        void updateSelection(QModelIndex candidate);
        void startRoutine();
        void stopRoutine();
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
        void drawRow(QPainter *painter, const QStyleOptionViewItem &options, const QModelIndex &index) const;
        void resizeEvent(QResizeEvent *);
        bool prepareDownloading(QString path);

        void downloadItem(ModelItem * item, QString savePath);
        void downloadBranch(ModelItem * rootNode, QString savePath);


        ModelItem * activeItem(bool next = true);
    //    ModelItem * nextItem(QModelIndex currIndex);
        ModelItem * nextItem(ModelItem * curr);
        ModelItem * prevItem(ModelItem * curr);

        void dragEnterEvent(QDragEnterEvent *event);
        void dragMoveEvent(QDragMoveEvent *event);
        virtual void dropEvent(QDropEvent *event);

        void keyPressEvent(QKeyEvent *event);
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent * event);

        ModelInterface * mdl;
        ViewSettings sttgs;
        QPoint dragStartPoint;
    };
}

#endif // VIEW_H
