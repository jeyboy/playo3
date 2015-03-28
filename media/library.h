#ifndef LIBRARY_H
#define LIBRARY_H

#include <qabstractitemmodel.h>
#include <qfile.h>
#include <qmutex.h>
#include <qtimer.h>
#include <qfuturewatcher.h>
#include <QtConcurrent/QtConcurrent>

//#include <QHash>
//#include <QThread>
//#include <QApplication>
//#include <QVector>

//#include "media/media_player/utils/genre.h"
#include "mediainfo.h"
#include "misc/file_utils/filename_conversions.h"
#include "media/format.h"
#include "media/duration.h"
//#include "misc/func_container.h"
#include "modules/data_struct/model_interface.h"

namespace Playo3 {
    class Library : public QObject {
        Q_OBJECT
    public:
        static Library * instance(QObject * parent = 0);

        void restoreItemState(const QModelIndex & ind);
        void declineItemState(const QModelIndex & ind);

        inline void setWaitListLimit(int newLimit) { waitListLimit = newLimit; }
    signals:
        void updateAttr(const QModelIndex, int attr, QVariant val);
    private slots:
        void initStateRestoring();
        void finishStateRestoring();
        void initRemoteItemInfo();
        void finishRemoteItemInfoInit();

        void clockTick();
        void saveCatalogs();
    private:
        static Library * self;

        Library(QObject * parent);

        ~Library();

        inline QString libraryPath() { return QCoreApplication::applicationDirPath() + "/library/"; }

        bool proceedItemNames(QStringList & names, int state);
        QChar getCatalogName(QString name);

        QHash<QString, int> * getCatalog(QChar & letter);
        QHash<QString, int> * getCatalog(QString & name);

        void stateRestoring(QModelIndex ind);
        void remoteInfoRestoring(QModelIndex ind);

        void initItemData(IItem * itm);
        void initItemInfo(MediaInfo & info, IItem * itm);
        void initItemTitles(MediaInfo & info, IItem * itm);

        QHash<QString, int> * load(const QChar letter);
        void save();
        bool fileDump(QChar key, QList<QString> & keysList, QFlags<QIODevice::OpenModeFlag> openFlags);

        QHash<QChar, QHash<QString, int>* > catalogs;
        QHash<QChar, QList<QString> *> catalogs_state;

        QList<QModelIndex> waitOnProc;
        QHash<QModelIndex, QFutureWatcher<void> * > inProc;

        QList<QModelIndex> waitRemoteOnProc;
        QHash<QModelIndex, QFutureWatcher<void> * > inRemoteProc;

        QTimer * saveTimer;
        QMutex saveBlock, itmAttrsBlock;

        QFuture<void> catsSaveResult;

        int waitListLimit;
        int ticksAmount;
    };
}

#endif // LIBRARY_H
