#ifndef ITRACKABLE
#define ITRACKABLE

#include <qwidget.h>

#ifdef Q_OS_WIN
    #include <qwintaskbarbutton.h>
    #include <qwintaskbarprogress.h>
#endif

class ITrackable {
public:
    virtual ~ITrackable() {}

    virtual QString title() const = 0;
protected:
    ITrackable(QWidget * parent);

    void updateState(bool played, bool paused, bool stopped);
    void setProgress(int pos);
    void setMaxProgress(int maxPos);
private:
    #ifdef Q_OS_WIN
        QWinTaskbarButton * stateButton;
        QWinTaskbarProgress * stateProgress;
    #endif
};

#endif // PLAYER_H
