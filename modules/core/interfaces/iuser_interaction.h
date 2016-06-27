#ifndef IUSER_INTERACTION
#define IUSER_INTERACTION

#include <qurl.h>
#include "dialogs/user_action_dialog.h"

namespace Core {
        class IUserInteraction {
        public:
            IUserInteraction();
            inline virtual ~IUserInteraction() {}
        protected:
            UserActionDialog * actionDialog;

            bool showingCaptcha(const QUrl & pict_url, QString & result);
            bool showingLogin(const QString & title, QString & login, QString & pass, const QString & err = QString());
            bool showingLoginWithCaptcha(const QString & title, const QUrl & pict_url, QString & login, QString & pass, QString & captcha, const QString & err = QString());
        };
}

#endif // IUSER_INTERACTION
