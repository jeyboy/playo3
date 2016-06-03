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

    QHash<QString, bool> orders;
    QHash<QString, QModelIndex> packet_requests;
    QHash<QModelIndex, int> packet_limiters;

    QHash<QString, QLabel *> requests;
    QHash<QString, QString> locale_pathes;

    ImageBank() {
        QDir dir(bankPath());
        if (!dir.exists())
            dir.mkpath(".");
    }

    friend class Core::Singleton<ImageBank>;
public:
    QString bankPath() { return QCoreApplication::applicationDirPath() % QStringLiteral("/temp_picts/"); }

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

            if (!orders.contains(url)) {
                orders.insert(url, true);
                Core::Web::Manager::prepare() -> followedGetAsync(QUrl(url), Func(this, SLOT(pixmapDownloaded(Response*,void*))));
            }
        }
    }

    void proceedPacket(const QModelIndex & ind, const QStringList & urls) {
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
                packet_limiters[ind]++;
                packet_requests.insertMulti(*url, ind);

                if (!orders.contains(*url)) {
                    orders.insert(*url, true);
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
        QString url_str = url.toString();
        QString filename = bankPath() % QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss.zzz-") % url.fileName();

        bool has_errors = response -> hasErrors();
        QPixmap pix = response -> toPixmap();

        if (!(has_errors || pix.isNull())) {
            QSize pix_size = pix.size();
            QSize max_size = FEED_BANK_IMG_MAX_SIZE;

            if (pix_size.width() > max_size.width() || pix_size.height() > max_size.height())
                pix = pix.scaled(FEED_BANK_IMG_MAX_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);

            if (pix.save(filename, 0, 100)) // without compression
                locale_pathes.insert(url_str, filename);
            else
                qDebug() << "File is not saved" << filename;
        }
        else qDebug() << "Pixmap is wrong";

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

        orders.remove(url_str);
    }
};

#endif // IMAGE_BANK
