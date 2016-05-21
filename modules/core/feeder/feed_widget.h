#ifndef FEED_WIDGET
#define FEED_WIDGET

#include <qwidget.h>
#include <QGridLayout>
#include <QHBoxLayout>
#include <qlabel.h>
#include <qdebug.h>
#include <qstyle.h>
#include <qdesktopservices.h>

#include "feeder.h"

#define PICTS_LIMIT 5

class FeedWidget : public QWidget {
    Q_OBJECT

    QUrl url;
public:
    FeedWidget(const FeedItem * item, QWidget * parent = 0) : QWidget(parent), url(item -> link) {
        QGridLayout * l = new QGridLayout(this);

        QLabel * title = new QLabel(item -> title, this);
        l -> addWidget(title, 0, 0);


        int row = 1;
        for(int count = 0; count < item -> images.size(); row++, count += PICTS_LIMIT) {
            QWidget * images_panel = new QWidget(this);
            QHBoxLayout * hl = new QHBoxLayout(images_panel);

            int sub_limit = qMin(PICTS_LIMIT, item -> images.size() - count);
            for(int i = 0; i < sub_limit; i++) {
                QLabel * img = new QLabel(images_panel);
                img -> setScaledContents(true);
                img -> setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Expanding);
                hl -> addWidget(img);
//                Core::Web::Manager::prepare() -> getImageAsync(item -> images[count + i], Func(img, SLOT(setPixmap(QPixmap))));
                ImageBank::obj().proceed(img, item -> images[count + i]);
            }

            l -> addWidget(images_panel, row, 0);
        }

        QLabel * desc = new QLabel(item -> desc, this);
        l -> addWidget(desc, row, 0);
    }
protected:
    void mouseDoubleClickEvent(QMouseEvent *) {
        QDesktopServices::openUrl(url);
    }
};

#endif // FEED_WIDGET
