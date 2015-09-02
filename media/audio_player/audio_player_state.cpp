#include "audio_player_state.h"

using namespace AudioPlayer;

//void endTrackSync(HSYNC handle, DWORD channel, DWORD data, void * user)
void endTrackSync(HSYNC, DWORD, DWORD, void * user) {
//    BASS_ChannelStop(channel);
//    BASS_ChannelRemoveSync(channel, handle);
    State * player = static_cast<State *>(user);
    emit player -> playbackEnded();
}

void endTrackDownloading(HSYNC, DWORD, DWORD, void * user) {
    State * player = static_cast<State *>(user);
    player -> finishRemoteFileDownloading();
    emit player -> downloadEnded();
}

//from 0 to 1
float State::getRemoteFileDownloadPosition() {
    if (size == -1) {
        prevDownloadPos = 0;
        DWORD len = BASS_StreamGetFilePosition(chId(), BASS_FILEPOS_END);
        size = len + BASS_StreamGetFilePosition(chId(), BASS_FILEPOS_START);
    }

    if (prevDownloadPos != 1) {
        prevDownloadPos = ((BASS_StreamGetFilePosition(chId(), BASS_FILEPOS_DOWNLOAD)) / size);
    }
    return prevDownloadPos;
}
