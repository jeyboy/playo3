#ifndef PLAYER_INDEX
#define PLAYER_INDEX

#include <qlist.h>

#include "player_callback.h"
#include "bass_player.h"

enum PlayerType {
    none = 0,
    bass_player = 1
};

class PlayerFactory : public Core::Singleton<PlayerFactory> {
    QList<PlayerCallback> callbacks;

    friend class Core::Singleton<PlayerFactory>;
    inline PlayerFactory() {}
public:
    void registerCallback(bool input, QObject * obj, const char * signal, const char * slot) {
        callbacks << PlayerCallback(input, obj, signal, slot);
    }

    IPlayer * build(const PlayerType & newPlayerType) {
        IPlayer * player;

        switch(newPlayerType) {
    //        case bass_player: break;
            default: player = new BassPlayer(anchor);
        }

        for(QList<PlayerCallback>::Iterator cl = callbacks.begin(); cl != callbacks.end(); cl++)
            (*cl).use(player);

        return player;
    }
};

#endif // PLAYER_INDEX
