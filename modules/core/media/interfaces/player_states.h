#ifndef PLAYER_STATES
#define PLAYER_STATES

#include <qflags.h>

enum PlayerStateFlags {
    InitState,
    StoppedState,
    PlayingState,
    PausedState,
    UnknowState
};
typedef QFlags<PlayerStateFlags> PlayerState;

#endif // PLAYER_STATES
