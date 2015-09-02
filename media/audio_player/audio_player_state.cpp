#include "audio_player_state.h"

//void endTrackSync(HSYNC handle, DWORD channel, DWORD data, void * user)
void endTrackSync(HSYNC, DWORD, DWORD, void * user) {
//    BASS_ChannelStop(channel);
//    BASS_ChannelRemoveSync(channel, handle);
    AudioPlayerState * player = static_cast<AudioPlayerState *>(user);
    emit player -> playbackEnded();
}

void endTrackDownloading(HSYNC, DWORD, DWORD, void * user) {
    AudioPlayerState * player = static_cast<AudioPlayerState *>(user);
    player -> finishRemoteFileDownloading();
    emit player -> downloadEnded();
}


