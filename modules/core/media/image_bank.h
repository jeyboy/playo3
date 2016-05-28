#ifndef IMAGE_BANK
#define IMAGE_BANK

#include <qobject.h>
#include <qhash.h>
#include <qurl.h>
#include <qlabel.h>
#include <qdir.h>
#include <qicon.h>

#include "modules/core/web/utils/web_manager.h"
#include "modules/core/interfaces/singleton.h"

#define FEED_BANK_IMG_MAX_SIZE QSize(640, 480)

using namespace Core::Web;

class ImageBank : public QObject, public Core::Singleton<ImageBank> {
    Q_OBJECT

    QHash<QUrl, bool> orders;
    QHash<QUrl, QModelIndex> packet_requests;
    QHash<QModelIndex, int> packet_limiters;

    QHash<QUrl, QLabel *> requests;
    QHash<QUrl, QUrl> locale_pathes;

    ImageBank() {
        QDir dir(bankPath());
        if (!dir.exists())
            dir.mkpath(".");
    }

    friend class Core::Singleton<ImageBank>;
public:
    QString bankPath() { return QCoreApplication::applicationDirPath() % QStringLiteral("/temp_picts/"); }

    bool hasImage(const QUrl & url) {
        return locale_pathes.contains(url);
    }

    QPixmap pixmap(const QUrl & url) {
        if (locale_pathes.contains(url))
            return QPixmap(locale_pathes[url].toString());
        else
            return QPixmap();
    }

    QIcon icon(const QUrl & url) {
        return QIcon(pixmap(url));
    }

    void proceed(QLabel * obj, const QUrl & url) {
        if (locale_pathes.contains(url)) {
            obj -> setPixmap(QPixmap(locale_pathes.value(url).toLocalFile()));
        } else {
            if (!requests.contains(url)) {
                requests.insert(url, obj);

                if (!orders.contains(url)) {
                    orders.insert(url, true);
                    Core::Web::Manager::prepare() -> followedGetAsync(url, Func(this, SLOT(pixmapDownloaded(Response*,void*))));
                }
            }
        }
    }

    void proceedPacket(const QModelIndex & ind, const QStringList & urls) {
        bool ready = true;

        for(QStringList::ConstIterator url = urls.cbegin(); url != urls.cend(); url++) {
            QUrl u = QUrl(*url);
            if (!locale_pathes.contains(u)) {
                ready = false;
                packet_limiters[ind]++;
                packet_requests.insertMulti(u, ind);

                if (!orders.contains(u)) {
                    orders.insert(u, true);
                    Core::Web::Manager::prepare() -> followedGetAsync(u, Func(this, SLOT(pixmapDownloaded(Response*,void*))));
                }
            }
        }

        if (ready)
            emit const_cast<QAbstractItemModel *>(ind.model()) -> dataChanged(ind, ind);
    }
public slots:
    void pixmapDownloaded(Response * response, void * /*user_data*/) {
        QUrl url = response -> url();
        QString filename = bankPath() % QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss.zzz-") % url.fileName();

        QPixmap pix = response -> toPixmap();
        QSize pix_size = pix.size();
        QSize max_size = FEED_BANK_IMG_MAX_SIZE;

        if (pix_size.width() > max_size.width() || pix_size.height() > max_size.height())
            pix = pix.scaled(FEED_BANK_IMG_MAX_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        if (!pix.save(filename, 0, 100)) // without compression
            qDebug() << "File is not saved" << filename;

        QLabel * obj = requests.take(url);
        if (obj)
            obj -> setPixmap(pix);

        QModelIndex ind = packet_requests[url];

        if (ind.isValid()) {
            if (--packet_limiters[ind] <= 0) {
                packet_limiters.remove(ind);
                packet_requests.remove(url);

                emit const_cast<QAbstractItemModel *>(ind.model()) -> dataChanged(ind, ind);
            }
        }

        orders.remove(url);
    }
};

#endif // IMAGE_BANK
