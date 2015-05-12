#ifndef SEARCH_BAR
#define SEARCH_BAR

#include "modules/controls/clickable_label.h"

#include <qlineedit.h>
#include <qevent.h>
#include <QHBoxLayout>

namespace Playo3 {
    class SearchBar : public QWidget {
        Q_OBJECT
    public:
        SearchBar(
            const QObject * receiver, const char * search_start_slot, const char * search_end_signal,
            const QObject * closeReceiver, const char * close_slot, QWidget * parent = 0
        );
    public slots:
        void searchEnded();
    signals:
        void searchCalled(QString predicate);
    private slots:
        void initiateSearch();

    private:
        QLineEdit * text;
    };
}

#endif // SEARCH_BAR
