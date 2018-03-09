#include "player_index.h"

#include "settings.h"
#include "bass_player.h"
#include "qt_player.h"

IPlayer * PlayerFactory::setCurrentPlayer(const IPlayer::DriverId & new_driver_id) {
    if (player && player -> uid() == new_driver_id)
        return player;

    IPlayer * old_player = player;

    player = build(new_driver_id);

    for(QList<PlayerCallback>::Iterator cl = callbacks.begin(); cl != callbacks.end(); cl++)
        (*cl).use(player);

    player -> setVideoOutput(video_output);

    //TODO: connect other settings

    if (old_player) {
        player -> setVolume(old_player -> volume());
        player -> setPan(old_player -> pan());
        player -> activateEQ(old_player -> eqInUse());
        player -> eqGains(old_player -> eqGains(), true);
        player -> setMedia(old_player -> mediaUrl(), old_player -> title(), old_player -> startPosition(), old_player -> duration(), old_player -> position());

        delete old_player;
    }

    return player;
}

IPlayer * PlayerFactory::build(const IPlayer::DriverId & driver_id) {
    if (players.contains(driver_id))
        return players.value(driver_id);

    QWidget * anchor = Settings::obj().anchorWidget();

    Q_ASSERT(anchor != 0);

    switch(driver_id) {
        case IPlayer::driver_id_qt: {
            return (IPlayer *)*(players.insert(driver_id, new QtPlayer(anchor)));
        }

        default: {
            if (players.contains(IPlayer::driver_id_bass))
                return players.value(IPlayer::driver_id_bass);

            return (IPlayer *)*(players.insert(IPlayer::driver_id_bass, new BassPlayer(anchor)));
        }
    }
}
