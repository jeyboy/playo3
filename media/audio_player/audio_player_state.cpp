#include "audio_player_state.h"

namespace AudioPlayer {
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

    void State::proceedErrorState() {
        currentState = UnknowState;
        switch(BASS_ErrorGetCode()) {
            case BASS_ERROR_FILEFORM: {
                emit mediaStatusChanged(InvalidMedia);
            break;}
            case BASS_ERROR_FILEOPEN: {
                emit mediaStatusChanged(NoMedia);
            break;}
            default: emit mediaStatusChanged(StalledMedia);
        }
    }
}
