#ifndef ITRACKABLE
#define ITRACKABLE

#include <qwidget.h>

#include "player_states.h"

#ifdef Q_OS_WIN
    #include <qwintaskbarbutton.h>
    #include <qwintaskbarprogress.h>
#endif

class ITrackable : public QObject {
    Q_OBJECT
public:
    virtual ~ITrackable();

    virtual QUrl mediaUrl() const = 0;
    virtual QString title() const = 0;

public slots:
    void updateStatus(const PlayerState & state);

protected:
    ITrackable(QWidget * parent);

    void setProgress(const int & pos);
    void setMaxProgress(const int & max_pos);
private:
    #ifdef Q_OS_WIN
        QIcon * ico_played;
        QIcon * ico_paused;

        QWinTaskbarButton * state_button;
        QWinTaskbarProgress * state_progress;
    #endif
};

#endif // PLAYER_H
