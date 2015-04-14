#ifndef DOCKBARS_H
#define DOCKBARS_H

#include <qmessagebox.h>
#include <QDockWidget>
#include <QBoxLayout>

#include "misc/file_utils/data_store.h"
#include "modules/controls/dockbar.h"
#include "modules/data_struct/view_index.h"
#include "modules/controls/window.h"

#include "dialogs/tabdialog.h"

namespace Playo3 {
    class Dockbars : public QWidget {
        Q_OBJECT
    public:
        ~Dockbars() {

        }

        static Dockbars * instance(QWidget * parent = 0);
        static void close() {
            delete self;
        }

        static inline QString settingsName() { return "docks"; }

        void load(QJsonArray & bars);
        void save(DataStore * settings);
        QDockWidget * linkNameToToolbars(QString barName, ViewSettings settings, QJsonObject attrs);

        inline QList<DockBar *> dockbars() { return parent() -> findChildren<DockBar *>(); }
        inline DockBar * current() const { return active; }
        inline void setActive(DockBar * bar) { active = bar; }
        inline void setPlayed(DockBar * bar) { played = bar; }
        inline void activate(DockBar * bar) {
            if ((active = bar))
                bar -> raise();
        }

        DockBar * commonBar();
        DockBar * createDocBar(QString name, ViewSettings settings, QJsonObject * attrs = 0, bool closable = true);
        DockBar * createDocBar(QString name, bool closable = true, QWidget * content = 0);

        inline IView * view(DockBar * bar) { return bar ? qobject_cast<IView *>(bar -> mainWidget()) : 0; }
        void useVeticalTitles(bool vertical);
    public slots:
        void updateAllViews();
        void hideAll();
        void showAll();

        inline void createNewBar() { showViewSettingsDialog(); }
        void editActiveBar() { showViewSettingsDialog(active); }
        inline void scrollToActive() {
            IView * v = view(active);
            if (v) v -> scrollToActive();
        }

        void onNextItemNeeded(Player::Reason);

        void nextExecTriggering();
        void nextExecWithDelTriggering();
        void prevExecWithDelTriggering();
        void prevExecTriggering();

    protected:
        void initPlayed();
        void showViewSettingsDialog(DockBar * bar = 0);

    private slots:
        void barClosed();
    private:
        DockBar * active, * played, * common;

        Dockbars(QWidget * parent) : QWidget(parent), active(0), played(0), common(0) {
            connect(Player::instance(), SIGNAL(nextItemNeeded(Player::Reason)), this, SLOT(onNextItemNeeded(Player::Reason)));
        }

        static Dockbars * self;
    };
}

#endif // DOCKBARS_H
