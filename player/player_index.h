#ifndef PLAYER_INDEX
#define PLAYER_INDEX

#include <qlist.h>

#include "modules/core/media/interfaces/iplayer.h"
#include "player_callback.h"

#include "settings.h"

enum PlayerDriver {
    driver_bass = 0,
    driver_qt
};

enum PlayerInitState {
    played,
    paused,
    initiated
};

class PlayerFactory : public Core::Singleton<PlayerFactory> {
    QList<PlayerCallback> callbacks;

    friend class Core::Singleton<PlayerFactory>;
    inline PlayerFactory() : player(0), video_output(0) {}
    inline ~PlayerFactory() {}

    IPlayer * player;
    QObject * video_output;
    QWidget * anchor_obj;
public:
    inline IPlayer * currPlayer() { return player; }

    QHash<QString, int> availableDrivers() {
        QHash<QString, int> res;

        res.insert(QStringLiteral("Bass"), driver_bass);
        res.insert(QStringLiteral("Qt"), driver_qt);

        return res;
    }

    void registerVideoOutput(QObject * new_video_output) {
        video_output = new_video_output;
        if (player && player -> isSupportVideo())
            player -> setVideoOutput(video_output);
    }

    void registerCallback(const CallbackTurn & turn, QObject * obj, const char * signal, const char * slot) {
        PlayerCallback pl(turn, obj, signal, slot);
        callbacks << pl;
        if (player) pl.use(player);
    }

    IPlayer * build(const PlayerDriver & new_driver, QWidget * anchor = 0);
};

#endif // PLAYER_INDEX
