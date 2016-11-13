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

    virtual QString mediaUrl() const = 0;
protected:
    ITrackable(QWidget * parent);

    void updateState(const bool & played, const bool & paused, const bool & stopped, const bool & visible = true);
    void setProgress(const int & pos);
    void setMaxProgress(const int & max_pos);
private:
    #ifdef Q_OS_WIN
        QWinTaskbarButton * stateButton;
        QWinTaskbarProgress * stateProgress;
    #endif
};

#endif // PLAYER_H
