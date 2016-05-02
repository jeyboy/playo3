#ifndef SEARCH_BAR
#define SEARCH_BAR

#include "modules/controls/clickable_label.h"

#include <qlineedit.h>
#include <qevent.h>
#include <qpixmap.h>
#include <qboxlayout.h>

namespace Controls {
    class SearchBar : public QWidget {
        Q_OBJECT
    public:
        SearchBar(
            const QObject * receiver, const char * search_start_slot, const char * search_end_slot, const char * search_end_signal,
            const QObject * closeReceiver, const char * close_slot, QWidget * parent = 0
        );

        void setFocus() { text -> setFocus(); }
    public slots:
        void searchEnded();
        void onHide();

    private:
        QLineEdit * text;
    };
}

#endif // SEARCH_BAR
