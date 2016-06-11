#ifndef PLAYER_CALLBACK
#define PLAYER_CALLBACK

#include <qobject.h>

enum CallbackTurn {
    call_in = 1,
    call_out
};

class PlayerCallback {
    const char * _signal;
    const char * _slot;
    QObject * _obj;
    CallbackTurn _turn;
public:
    PlayerCallback(CallbackTurn turn, QObject * obj, const char * signal, const char * slot) : _signal(signal), _slot(slot), _obj(obj), _turn(turn) {}
    void use(QObject * player) {
        if (_turn == call_in)
            QObject::connect(_obj, _signal, player, _slot);
        else
            QObject::connect(player, _signal, _obj, _slot);
    }
};

#endif // PLAYER_CALLBACK
