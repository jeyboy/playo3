#ifndef OD_RADIO
#define OD_RADIO

#include "od_defines.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Radio : public Base {
            protected:
                inline QString tunersUrl() { return audioUrlStr(path_audio_tuners_for_radio); } // params: (locale: 'ru')  need to check pagination
                inline QString radioUrl(QString /*tuner*/) { return audioUrlStr(path_audio_radio); } // params: (locale: 'ru') (tuner: taked from tunersUrl() returned list) and pagination attrs
            };
        }
    }
}

#endif // OD_RADIO
