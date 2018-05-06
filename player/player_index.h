#ifndef PLAYER_INDEX
#define PLAYER_INDEX

#include <qlist.h>

#include "modules/core/media/interfaces/iplayer.h"
#include "player_callback.h"

class PlayerFactory : public Core::Singleton<PlayerFactory> {
    QList<PlayerCallback> callbacks;

    QHash<IPlayer::DriverId, IPlayer *> players;

    friend class Core::Singleton<PlayerFactory>;
    inline PlayerFactory() : player(0), video_output(0) {}
    inline ~PlayerFactory() {}

    IPlayer * player;
    QObject * video_output;

    IPlayer * build(const IPlayer::DriverId & new_driver_id);
public:
    inline IPlayer * currPlayer() { return player; }

    QHash<QString, int> availableDrivers() {
        QHash<QString, int> res;

        res.insert(QStringLiteral("Bass"), IPlayer::driver_id_bass);
        res.insert(QStringLiteral("Qt"), IPlayer::driver_id_qt);

        return res;
    }

    QHash<QString, QVariant> availableOutputDevices(const IPlayer::DriverId & driver_id = IPlayer::driver_id_active) {
        IPlayer * target_player = build(driver_id);
        // error on change player while item played: qt to bass
        Q_ASSERT(target_player);
        return target_player -> outputDeviceList();
    }

    void registerVideoOutput(QObject * new_video_output) {
        video_output = new_video_output;
        if (player && player -> isSupportVideo()) {
             player -> setVideoOutput(video_output);
        }
    }

    void registerCallback(const CallbackTurn & turn, QObject * obj, const char * signal, const char * slot) {
        PlayerCallback pl(turn, obj, signal, slot);
        callbacks << pl;
        if (player) pl.use(player);
    }

    IPlayer * setCurrentPlayer(const IPlayer::DriverId & new_driver_id);
};

#endif // PLAYER_INDEX
