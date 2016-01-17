#ifndef LIBRARY_BASE
#define LIBRARY_BASE

#include <qstringbuilder.h>
#include <qmutex.h>
#include <qtimer.h>
#include <qfile.h>
#include <qfuturewatcher.h>
#include <QtConcurrent/QtConcurrent>
#include <qdebug.h>

#include "modules/core/misc/logger.h"

#define TIMER_TICK 500

namespace Core {
    namespace Media {
        class LibraryBase : public QObject {
            Q_OBJECT
        public:
            LibraryBase();
            ~LibraryBase();
        protected slots:
            void saveCatalogs();
        protected:
            QChar getCatalogName(const QString & name);

            QHash<QString, int> * getCatalog(const QChar & letter);
            QHash<QString, int> * getCatalog(const QString & name);
            QHash<QString, int> * load(const QChar & letter);
            void save();
            bool fileDump(const QChar & key, QList<QString> & keysList, QFlags<QIODevice::OpenModeFlag> openFlags);

            QHash<QChar, QHash<QString, int> *> catalogs;
            QHash<QChar, QList<QString> *> catalogs_state;

            QTimer * saveTimer;
            QMutex saveBlock;

            int timeAmount;
            QString libraryPath;
        };
    }
}

#endif // LIBRARY_BASE
