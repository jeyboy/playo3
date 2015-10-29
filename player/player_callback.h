#ifndef PLAYER_CALLBACK
#define PLAYER_CALLBACK

#include <qobject.h>

class PlayerCallback {
    const char * _signal;
    const char * _slot;
    QObject * _obj;
    bool in;
public:
    PlayerCallback(bool input, QObject * obj, const char * signal, const char * slot) : _signal(signal), _slot(slot), _obj(obj), in(input) {}
    void use(QObject * player) {
        if (in)
            QObject::connect(_obj, _signal, player, _slot);
        else
            QObject::connect(player, _signal, _obj, _slot);
    }
};

#endif // PLAYER_CALLBACK
