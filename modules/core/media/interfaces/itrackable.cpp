#include "itrackable.h"

#include <qdebug.h>

ITrackable::ITrackable(QWidget * parent) : ico_played(0), ico_paused(0) {
    ico_played = new QIcon(QLatin1String(":task_play"));
    ico_paused = new QIcon(QLatin1String(":task_pause"));

    #ifdef Q_OS_WIN
        state_button = new QWinTaskbarButton(parent);
        parent -> winId(); // generate native object for windowHandle()
        state_button -> setWindow(parent -> windowHandle());
        state_progress = state_button -> progress();
        state_progress -> setMinimum(0);
    #else
        Q_UNUSED(parent);
    #endif
}

ITrackable::~ITrackable() {
    delete ico_played;
    delete ico_paused;
}

void ITrackable::updateStatus(const PlayerState & state) {
    bool played = state == PlayingState;
    bool paused = state == PausedState;
    bool visible = state != UnknowState;

    #ifdef Q_OS_WIN
        state_progress -> setVisible(visible);

        if (!played && !paused) {
            if (state == StoppedState) {
                state_button -> setOverlayAccessibleDescription(QLatin1String());
                state_button -> clearOverlayIcon();
            }
        } else {
            if (played) {
                state_button -> setOverlayIcon(*ico_played);
                state_button -> setOverlayAccessibleDescription(title());
                state_progress -> resume();
            }
            else if (paused) {
                state_button -> setOverlayIcon(*ico_paused);
                state_progress -> pause();
            }
        }
    #else
        Q_UNUSED(state);
    #endif
}

void ITrackable::setProgress(const int & pos) {
    #ifdef Q_OS_WIN
        state_progress -> setValue(pos);
    #else
        Q_UNUSED(pos);
    #endif
}

void ITrackable::setMaxProgress(const int & max_pos) {
    #ifdef Q_OS_WIN
        state_progress -> setVisible(true);
        state_progress -> setMaximum(max_pos);
    #else
        Q_UNUSED(maxPos);
    #endif
}
