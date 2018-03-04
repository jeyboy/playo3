#include "itrackable.h"

ITrackable::ITrackable(QWidget * parent) {
    #ifdef Q_OS_WIN
        stateButton = new QWinTaskbarButton(parent);
        parent -> winId(); // generate native object for windowHandle()
        stateButton -> setWindow(parent -> windowHandle());
        stateProgress = stateButton -> progress();
        stateProgress -> setMinimum(0);
    #else
        Q_UNUSED(parent);
    #endif
}

void ITrackable::updateState(const bool & played, const bool & paused, const bool & stopped, const bool & visible) {
    #ifdef Q_OS_WIN
        Q_UNUSED(stopped);
        stateProgress -> setVisible(visible);

        if (!played && !paused)
            stateButton -> setOverlayIcon(QIcon());
        else
            stateButton -> setOverlayIcon(QIcon(
                played ? QStringLiteral(":task_play") : QStringLiteral(":task_pause")
            ));

        if (played) {
            stateProgress -> resume();
            stateButton -> setOverlayAccessibleDescription(title());
        }

        if (paused)
            stateProgress -> pause();
    #else
        Q_UNUSED(played);
        Q_UNUSED(paused);
        Q_UNUSED(stopped);
        Q_UNUSED(visible);
    #endif
}

void ITrackable::setProgress(const int & pos) {
    #ifdef Q_OS_WIN
        stateProgress -> setValue(pos);
    #else
        Q_UNUSED(pos);
    #endif
}

void ITrackable::setMaxProgress(const int & max_pos) {
    #ifdef Q_OS_WIN
        stateProgress -> setVisible(true);
        stateProgress -> setMaximum(max_pos);
    #else
        Q_UNUSED(maxPos);
    #endif
}
