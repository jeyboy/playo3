#include "dockbar.h"

using namespace Playo3;

DockBar::DockBar(const QString &title, QWidget *parent, Qt::WindowFlags flags) : QDockWidget(title, parent, flags) {
    setTitleBarWidget(new WindowTitle(this, 30, QMargins(0,0,0,0), 0));
}
