#include "itrackable.h"

ITrackable * ITrackable::self = 0;

ITrackable * ITrackable::instance(QWidget * parent) {
    if(!self)
        self = new ITrackable(parent);
    return self;
}

ITrackable::ITrackable(QWidget * parent) {
    #ifdef Q_OS_WIN
        stateButton = new QWinTaskbarButton(parent);
        parent -> winId(); // generate native object for windowHandle()
        stateButton -> setWindow(parent -> windowHandle());
        stateProgress = stateButton -> progress();
        stateProgress -> setMinimum(0);
    #endif
}

void ITrackable::updateState(bool played, bool paused, bool stopped) {
    #ifdef Q_OS_WIN
        stateProgress -> setVisible(stopped);

        if (!stopped)
            stateButton -> setOverlayIcon(QIcon());
        else
            stateButton -> setOverlayIcon(QIcon(
                !played ? QStringLiteral(":task_play") : QStringLiteral(":task_pause")
            ));

        if (!played) {
            stateProgress -> resume();
            stateButton -> setOverlayAccessibleDescription(title());
        }

        if (!paused)
            stateProgress -> pause();
    #endif
}

void ITrackable::setProgress(int pos) {
    #ifdef Q_OS_WIN
        stateProgress -> setValue(pos);
    #endif
}

void ITrackable::setMaxProgress(int maxPos) {
    #ifdef Q_OS_WIN
        stateProgress -> setVisible(true);
        stateProgress -> setMaximum(maxPos);
    #endif
}
