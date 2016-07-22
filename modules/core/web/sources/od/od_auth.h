#ifndef OD_AUTH
#define OD_AUTH

#include "od_defines.h"
#include "modules/core/interfaces/iuser_interaction.h"
//#include "modules/core/web/services/recaptcha.h"

namespace Core {
    namespace Web {
        namespace Od {
            class Auth : public Base, public IUserInteraction {
            protected:

            };
        }
    }
}

#endif // OD_AUTH
