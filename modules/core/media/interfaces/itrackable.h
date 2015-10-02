#ifndef ITRACKABLE
#define ITRACKABLE

#include <qobject.h>

#ifdef Q_OS_WIN
    #include <qwintaskbarbutton.h>
    #include <qwintaskbarprogress.h>
#endif

class ITrackable {
public:
    static ITrackable * instance(QWidget * parent = 0);
    inline static void close() { delete self; }
    virtual ~ITrackable() {}

    virtual QString title() = 0;
protected:
    void updateState(bool played, bool paused, bool stopped);
    void setProgress(int pos);
    void setMaxProgress(int maxPos);
private:
    ITrackable(QWidget * parent);

    static ITrackable * self;

    #ifdef Q_OS_WIN
        QWinTaskbarButton * stateButton;
        QWinTaskbarProgress * stateProgress;
    #endif
};

#endif // PLAYER_H
