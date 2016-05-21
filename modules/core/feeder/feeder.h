#ifndef FEEDER
#define FEEDER

#include <qobject.h>
#include <qthread.h>
#include <qlabel.h>
#include <qdatetime.h>
#include <qstringbuilder.h>

#include "web_manager.h"
#include "singleton.h"

#define FEED_SETTINGS_URL_PAGINATION_TEMPLATE QStringLiteral("^1")

#define FEED_SETTINGS_FROM QStringLiteral("from")
#define FEED_SETTINGS_TO QStringLiteral("to")
#define FEED_SETTINGS_STEP QStringLiteral("step")

#define FEED_SETTINGS_ITEM_SELECTOR QStringLiteral("item_selector")
#define FEED_SETTINGS_TITLE_SELECTOR QStringLiteral("title_selector")
#define FEED_SETTINGS_LINK_SELECTOR QStringLiteral("link_selector")
#define FEED_SETTINGS_IMAGE_SELECTOR QStringLiteral("image_selector")
#define FEED_SETTINGS_DESC_SELECTOR QStringLiteral("desc_selector")

#define FEED_SETTINGS_IMAGE_GALLERY_TEMPLATE QStringLiteral("image_gallery_template")
#define FEED_SETTINGS_IMAGE_GALLERY_FROM QStringLiteral("image_gallery_from")
#define FEED_SETTINGS_IMAGE_GALLERY_TO QStringLiteral("image_gallery_to")

#define FEED_STEP 2
#define FEED_BANK_IMG_MAX_SIZE QSize(640, 480)

using namespace Core::Web;

enum FeederType {
    feeder_rss,
    feeder_html
};

struct FeederSettings {
    FeederSettings(FeederType ftype, QString name, QString url, int freq, QString last_link = QString())
        : ftype(ftype), name(name), url(url), last_link(last_link), freq(freq) { }

    FeederType ftype;
    QString name;
    QString url;
    QString last_link;
    int freq;
    QHash<QString, QVariant> attrs;
};

struct FeedItem {
    qint64 timestamp;
    QString title;
    QUrl link;
    QList<QUrl> images;
    QString desc;
};

class ImageBank : public QObject, public Core::Singleton<ImageBank> {
    Q_OBJECT

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
    void proceed(QLabel * obj, const QUrl & url) {
        if (locale_pathes.contains(url)) {
            obj -> setPixmap(QPixmap(locale_pathes.value(url).toLocalFile()));
        } else {
            requests.insert(url, obj);
            Core::Web::Manager::prepare() -> followedGetAsync(url, Func(this, SLOT(pixmapDownloaded(Response*,void*))));
        }
    }
public slots:
    void pixmapDownloaded(Response * response, void * /*user_data*/) {
        QUrl url = response -> url();
        QLabel * obj = requests.take(url);
        QString filename = bankPath() % QDateTime::currentDateTime().toString("yyyy.MM.dd_hh.mm.ss.zzz-") % url.fileName();

        QPixmap pix = response -> toPixmap();
        QSize pix_size = pix.size();
        QSize max_size = FEED_BANK_IMG_MAX_SIZE;

        if (pix_size.width() > max_size.width() || pix_size.height() > max_size.height())
            pix = pix.scaled(FEED_BANK_IMG_MAX_SIZE, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        if (!pix.save(filename, 0, 100)) // without compression
            qDebug() << "File is not saved" << filename;

        obj -> setPixmap(pix);
    }
};

class FeederJob : public QObject {
    Q_OBJECT

    FeederSettings settings;
    mutable bool close_required;

    void extractImagesFromDesc(FeedItem * item) {
        QRegularExpression re("<img\\s+[^>]*src=\"([^\"]*)\"[^>]*>",  QRegularExpression::CaseInsensitiveOption);
        int offset = 0;

        while(true) {
            QRegularExpressionMatch match = re.match(item -> desc, offset);
            if (!match.hasMatch()) break;

            QString src = match.captured(1);
            src = item -> link.resolved(QUrl(src)).toString();

            item -> desc.replace((offset = match.capturedStart(0)), match.capturedLength(0), QStringLiteral("(%1)").arg(item -> images.size()));
            item -> images.append(QUrl(src));
        }
    }

    bool prepareTeplate(QString & str, const QString & _template, const QString & rtemplate = FEED_SETTINGS_URL_PAGINATION_TEMPLATE) {
        QRegularExpression re(_template,  QRegularExpression::CaseInsensitiveOption);
        int offset = 0;
        bool has_entry = false;

        while(true) {
            QRegularExpressionMatch match = re.match(str);
            bool has_match = match.hasMatch();
            has_entry |= has_match;
            if (!has_match) return has_entry;\
            int pos = 1;


            QString src = match.captured(pos);
            if (src.isEmpty())
                src = match.captured(--pos);

            str.replace((offset = match.capturedStart(pos)), match.capturedLength(pos), rtemplate);
        }
    }
signals:
    void newFeed(const QString & name, const FeedItem * feed);
public:
    FeederJob(const FeederSettings & settings) : settings(settings), close_required(false) {}
    void setFrequency(int secs = 60) { settings.freq = secs;}
    void close() { close_required = true; }
public slots:
    void proc() {
        int counter = settings.freq;

        while(!close_required) {
            if (counter >= settings.freq) {
                counter = 0;

                qint64 timestamp = QDateTime::currentMSecsSinceEpoch();
                QString new_last_link;

                for(int offset = settings.attrs.value(FEED_SETTINGS_FROM, 0).toInt(); offset <= settings.attrs.value(FEED_SETTINGS_TO, 0).toInt(); offset += settings.attrs.value(FEED_SETTINGS_STEP, 1).toInt()) {
                    QUrl url(QString(settings.url).replace(FEED_SETTINGS_URL_PAGINATION_TEMPLATE, QString::number(offset)));

                    Core::Web::Response * response = Core::Web::Manager::prepare() -> followedGet(url);
                    Core::Web::Html::Document doc = response -> toHtml();

//                    doc.output();

                    switch(settings.ftype) {
                        case feeder_rss: {
                            bool is_xml = doc.isXml();
                            Core::Web::Html::Set entries = is_xml ? doc.find("item") : doc.find(".entry");

                            for(Core::Web::Html::Set::Iterator entry = entries.begin(); entry != entries.end(); entry++) {
                                Core::Web::Html::Tag * tag = *entry;

                                FeedItem * item = new FeedItem();
                                item -> timestamp = timestamp;

                                Core::Web::Html::Tag * title_tag = tag -> findFirst("title");
                                if (title_tag)
                                    item -> title = title_tag -> text();

                                Core::Web::Html::Tag * link_tag = tag -> findFirst("link");
                                if (link_tag) {
                                    item -> link = QUrl(link_tag -> text());
                                    if (settings.last_link == item -> link.toString())
                                        break;
                                }

                                Core::Web::Html::Tag * desc_tag = tag -> findFirst("description");
                                if (desc_tag) {
                                    item -> desc = desc_tag -> text();
                                    extractImagesFromDesc(item);
                                }

                                if (new_last_link.isEmpty())
                                    new_last_link = item -> link.toString();

                                emit newFeed(settings.name, item);
                            }
                        break;}

                        case feeder_html: {
                            Core::Web::Html::Set entries = doc.find(settings.attrs.value(FEED_SETTINGS_ITEM_SELECTOR).toString().toStdString().c_str());

                            for(Core::Web::Html::Set::Iterator entry = entries.begin(); entry != entries.end(); entry++) {
                                Core::Web::Html::Tag * tag = *entry;

                                FeedItem * item = new FeedItem();
                                item -> timestamp = timestamp;

                                Core::Web::Html::Tag * title_tag = tag -> findFirst(settings.attrs.value(FEED_SETTINGS_TITLE_SELECTOR).toString().toStdString().c_str());
                                if (title_tag)
                                    item -> title = title_tag -> text();

                                Core::Web::Html::Tag * link_tag = tag -> findFirst(settings.attrs.value(FEED_SETTINGS_LINK_SELECTOR).toString().toStdString().c_str());
                                if (link_tag) {
                                    item -> link = QUrl(link_tag -> link());
                                    if (settings.last_link == item -> link.toString())
                                        break;
                                }


                                QString image_selector = settings.attrs.value(FEED_SETTINGS_IMAGE_SELECTOR).toString();

                                if (!image_selector.isEmpty()) {
                                    Core::Web::Html::Set images = tag -> find(image_selector.toStdString().c_str());
                                    for(Core::Web::Html::Set::Iterator image_tag = images.begin(); image_tag != images.end(); image_tag++) {

                                        QString src = (*image_tag) -> value("src");
                                        QString gallery_template = settings.attrs.value(FEED_SETTINGS_IMAGE_GALLERY_TEMPLATE).toString();
                                        bool ignore_src = !gallery_template.isEmpty();

                                        if (ignore_src) {
                                            if (prepareTeplate(src, gallery_template)) {
                                                for(int i = settings.attrs.value(FEED_SETTINGS_IMAGE_GALLERY_FROM).toInt(); i < settings.attrs.value(FEED_SETTINGS_IMAGE_GALLERY_TO).toInt(); i++) {
                                                    QString prev = QString(src).replace(FEED_SETTINGS_URL_PAGINATION_TEMPLATE, QString::number(i));
                                                    item -> images.append(QUrl(prev));
                                                }
                                            }
                                            else ignore_src = false;
                                        }

                                        if (!ignore_src)
                                            item -> images.append(QUrl(src));
                                    }
                                }

                                QString desc_selector = settings.attrs.value(FEED_SETTINGS_DESC_SELECTOR).toString();
                                if (!desc_selector.isEmpty()) {
                                    Core::Web::Html::Tag * desc_tag = tag -> findFirst(desc_selector.toStdString().c_str());
                                    if (desc_tag) {
                                        item -> desc = desc_tag -> text();
                                        extractImagesFromDesc(item);
                                    }
                                }

                                if (new_last_link.isEmpty())
                                    new_last_link = item -> link.toString();

                                emit newFeed(settings.name, item);
                            }
                        break;}
                    }
                }

                settings.last_link = new_last_link;
            }

            QThread::sleep(FEED_STEP);
            counter += FEED_STEP;
        }
    }
};

class Feeder : public QObject {
    Q_OBJECT
    QThread workerThread;
    FeederJob * job;
public:
    Feeder(QObject * obj, const char * feed_slot, const FeederSettings & settings) : QObject(obj) {
        job = new FeederJob(settings);
        job -> moveToThread(&workerThread);

        connect(&workerThread, SIGNAL(started()), job, SLOT(proc()));
        connect(&workerThread, SIGNAL(finished()), job, SLOT(deleteLater()));
        connect(job, SIGNAL(newFeed(QString,const FeedItem*)), obj, feed_slot);

        workerThread.start();
    }
    ~Feeder() {
        job -> close();
        workerThread.quit();
        workerThread.wait();
    }
};

#endif // FEEDER
