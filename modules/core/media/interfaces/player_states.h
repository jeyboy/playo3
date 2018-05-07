#ifndef PLAYER_STATES
#define PLAYER_STATES

enum PlayerState : unsigned char {
    UnknowState = 0,
    InitState,
    StoppedState,
    PlayingState,
    PausedState
};

#endif // PLAYER_STATES
