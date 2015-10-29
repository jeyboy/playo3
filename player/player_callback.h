#ifndef PLAYER_CALLBACK
#define PLAYER_CALLBACK

#include <qobject.h>

class PlayerCallback {
    const char * _signal;
    const char * _slot;
    QObject * _obj;
    bool in;
public:
    PlayerCallback(bool input, QObject * obj, const char * signal, const char * slot) : _signal(signal), slot(_slot), _obj(obj), in(input) {}
    void use(QObject * player) {
        if (in)
            QObject::connect(obj, signal, player, slot);
        else
            QObject::connect(player, signal, obj, slot);
    }
};

#endif // PLAYER_CALLBACK
