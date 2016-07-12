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
#include "modules/core/misc/thread_utils.h"

#define BANK_IMG_MAX_SIZE QSize(640, 480)
#define BANK_IMG_SLOTS_AMOUNT 8

using namespace Core::Web;

class ImageBank : public QObject, public Core::Singleton<ImageBank> {
    Q_OBJECT

    QString bank_temp_dir;

    mutable bool block_thread;
    int in_proc;
    QList<QString> orders;

    QHash<QString, QLabel *> requests;
    QHash<QString, QModelIndex> packet_requests;
    QHash<QModelIndex, int> packet_limiters;

    QHash<QString, QString> locale_pathes;

    ImageBank() : bank_temp_dir(QCoreApplication::applicationDirPath() % QStringLiteral("/temp_picts/")), block_thread(false), in_proc(0) {
        QDir dir(bank_temp_dir);
        if (!dir.exists())
            dir.mkpath(".");
    }

    ~ImageBank() { clearCache(); }

    friend class Core::Singleton<ImageBank>;

    Core::Web::Response * procImageCall(const QUrl & url) { return Core::Web::Manager::prepare() -> getFollowed(url); }

    void procOrder() {
        if (block_thread) return;

        while(!orders.isEmpty()) {
            if (in_proc < BANK_IMG_SLOTS_AMOUNT) {
                Core::ThreadUtils::obj().run(
                    this, &ImageBank::procImageCall,
                    QUrl(orders.takeLast()),
                    new Func(this, SLOT(pixmapDownloaded(Response*)))
                );

                in_proc++;
            } else break;
        }
    }
public:
    void clearCache() {
        QDir dir(bank_temp_dir);
        qDebug() << "REM BANK" << dir.removeRecursively();
    }

    bool hasImage(const QString & url) {
        return locale_pathes.contains(url);
    }

    QPixmap pixmap(const QString & url) {
        if (locale_pathes.contains(url)) {
            QString path = locale_pathes.value(url);
            return QPixmap(path);
        } else
            return QPixmap();
    }

    QIcon icon(const QString & url) {
        return QIcon(pixmap(url));
    }

    void proceed(QLabel * obj, const QString & url) {       
        if (locale_pathes.contains(url)) {
            obj -> setPixmap(pixmap(url));
            return;
        }

        QUrl u = QUrl(url);
        if (!u.isValid()) {
            locale_pathes.insert(url, QStringLiteral(":main"));
            obj -> setPixmap(pixmap(QStringLiteral(":main")));
            return;
        }

        if (!requests.contains(url)) {
            requests.insert(url, obj);
            orders.append(url);
        }

        procOrder();
    }

    void proceedPacket(const QModelIndex & ind, const QStringList & urls) { // TODO: need to use order with slide window
        if (packet_limiters.contains(ind)) return;

        bool ready = true;

        for(QStringList::ConstIterator url = urls.cbegin(); url != urls.cend(); url++) {
            QUrl u = QUrl(*url);

            if (!u.isValid()) {
                locale_pathes.insert(*url, QStringLiteral(":main"));
                continue;
            }

            if (!locale_pathes.contains(*url)) {
                ready = false;

                if (!packet_requests.contains(*url)) {
                    packet_limiters[ind]++;
                    packet_requests.insertMulti(*url, ind);
                    orders.append(*url);
//                    Core::Web::Manager::prepare() -> getFollowedAsync(u, Func(this, SLOT(pixmapDownloaded(Response*,void*))));
                }
            }
        }

        if (ready)
            emit const_cast<QAbstractItemModel *>(ind.model()) -> dataChanged(ind, ind);
        else
            procOrder();
    }

    void cancelPackets(QAbstractItemModel * model) {
        block_thread = true;

        QMutableHashIterator<QString, QModelIndex> i(packet_requests);
        while (i.hasNext()) {
            i.next();

            if (i.value().model() == model) {
                orders.removeAll(i.key());
                packet_limiters.remove(i.value());
                i.remove();
            }
        }

        block_thread = false;
        procOrder();
    }
public slots:
    void pixmapDownloaded(Response * response) {
        QUrl url = response -> url();
        QString url_str = url.toString();
        QString filename = bank_temp_dir % QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss.zzz-") % url.fileName();

        bool has_errors = response -> hasErrors();
        QPixmap pix = response -> toPixmap();

        if (!(has_errors || pix.isNull())) {
            QSize pix_size = pix.size();
            QSize max_size = BANK_IMG_MAX_SIZE;

            if (pix_size.width() > max_size.width() || pix_size.height() > max_size.height())
                pix = pix.scaled(BANK_IMG_MAX_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            if (pix.save(filename, 0, 100)) // without compression
                locale_pathes.insert(url_str, filename);
            else
                qDebug() << "File is not saved" << filename;
        }
        else {
            qDebug() << "Pixmap is wrong";
            locale_pathes.insert(url_str, QString()); // blocking retry of requests
        }

        QLabel * obj = requests.take(url_str);
        if (obj)
            obj -> setPixmap(pix);

        QModelIndex ind = packet_requests[url_str];

        if (ind.isValid()) {
            if (--packet_limiters[ind] <= 0) {
                packet_limiters.remove(ind);
                packet_requests.remove(url_str);

                emit const_cast<QAbstractItemModel *>(ind.model()) -> dataChanged(ind, ind);
            }
        }

        in_proc--;
        procOrder();
    }
};

#endif // IMAGE_BANK
