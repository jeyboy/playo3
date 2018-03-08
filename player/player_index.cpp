#include "player_index.h"

#include "bass_player.h"
#include "qt_player.h"

IPlayer * PlayerFactory::build(const PlayerDriver & new_driver, QWidget * anchor) {
    if (!anchor)
        anchor = anchor_obj;

    Q_ASSERT(anchor > 0);

    IPlayer * old_player = player;

    switch(new_driver) {
        case driver_qt: player = new QtPlayer(anchor); break;
        default: player = new BassPlayer(anchor);
    }

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
