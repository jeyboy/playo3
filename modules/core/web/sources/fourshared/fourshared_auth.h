#ifndef FOURSHARED_AUTH
#define FOURSHARED_AUTH

#include "fourshared_defines.h"

#include "modules/core/web/interfaces/auth/oauth.h"
#include "modules/core/interfaces/iuser_interaction.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Auth : public Base, public IUserInteraction {
            public:
            };
        }
    }
}

#endif // FOURSHARED_AUTH
