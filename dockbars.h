#ifndef DOCKBARS_H
#define DOCKBARS_H

#include <qmessagebox.h>
#include <QDockWidget>
#include <QBoxLayout>

#include "modules/core/interfaces/singleton.h"
#include "modules/core/misc/logger.h"
#include "modules/core/misc/file_utils/data_store.h"

#include "modules/views/view_index.h"

#include "modules/controls/window.h"
#include "modules/controls/dockbar.h"

#include "dialogs/tabdialog.h"

#define SCREEN_TAB "Screen"
#define COMMON_TAB "Common"
#define DOWNLOADS_TAB "Downloads"
#define LOGS_TAB "Logs"

namespace Presentation {
    using namespace Views;
    using namespace Controls;

    class Dockbars : public QWidget, public Singleton<Dockbars> {
        Q_OBJECT
    public:
        static inline QString settingsName() { return QStringLiteral("docks"); }

        void load(const QJsonArray & bars);
        void save(DataStore * settings);
        QDockWidget * linkNameToToolbars(QString barName, Params settings, QJsonObject attrs);

        inline QList<DockBar *> dockbars() { return container -> findChildren<DockBar *>(); }

        //        You can check if some part of dock widget is visible using the following code:
        //        bool really_visible = !widget->visibleRegion().isEmpty();
        //        I tested that result value depends on which dock widget tab is active.
        inline DockBar * current() const { return active; }

        inline void setActive(DockBar * bar) { active = bar; }
        inline void setPlayed(DockBar * bar) {
            if (bar == played) return;

            if ((played = bar))
                updateActiveTabIcon();
        }
        inline void activate(DockBar * bar) {
            if ((active = bar))
                bar -> raise();
        }

        DockBar * commonBar();
        DockBar * createDocBar(QString name, Params settings, QJsonObject * attrs = 0, bool closable = true, bool addToView = false, SearchSettings * search_settings = 0);
        DockBar * createDocBar(QString name, bool closable = true, QWidget * content = 0);
        DockBar * createLinkedDocBar(QString text, QString path, Params settings);

        inline IView * view(DockBar * bar) { return bar ? qobject_cast<IView *>(bar -> mainWidget()) : 0; }
        void useVeticalTitles(bool vertical);

        inline void setContainer(QMainWindow * ct) { container = ct; }
    public slots:
        void updateActiveTabIcon(bool isFloating = false);
        void updateAllViews();
        inline void hideAll() { changeBarsVisibility(true); }
        inline void showAll() { changeBarsVisibility(false); }

        inline void createNewBar() { showViewSettingsDialog(); }
        inline void editActiveBar() { showViewSettingsDialog(active); }
        void scrollToActive();

        void playNext();
        void playNextWithDel();
        void playPrevWithDel();
        void playPrev();
        void stop();
        void playPause();
        void slidePosForward();
        void slidePosBackward();
        void slideVolForward();
        void slideVolBackward();

    protected:
        void changeBarsVisibility(bool hide);
        void initPlayed();
        void showViewSettingsDialog(DockBar * bar = 0);

    private slots:
        void onDownloadProceeded(QString to);
        void barClosed();
    private:
        TabifyParams lastTabData;
        DockBar * active, * played, * common;
        QMainWindow * container;

        QHash<QString, DockBar *> linkedTabs;

        friend class Singleton<Dockbars>;
        inline Dockbars() : QWidget(), active(0), played(0), common(0) {}

        inline ~Dockbars() {}

        static Dockbars * self;
    };
}

#endif // DOCKBARS_H
