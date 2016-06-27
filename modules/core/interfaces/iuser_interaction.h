#ifndef IUSER_INTERACTION
#define IUSER_INTERACTION

#include "settings.h"
#include "dialogs/user_action_dialog.h"
#include "modules/core/web/utils/web_manager.h"


namespace Core {
        class IUserInteraction {
        public:
            inline IUserInteraction() { actionDialog = new UserActionDialog(Settings::obj().anchorWidget()); }
            inline virtual ~IUserInteraction() {}
        protected:
            UserActionDialog * actionDialog;

            bool showingCaptcha(const QUrl & pict_url, QString & result);
            bool showingLogin(const QString & title, QString & login, QString & pass, const QString & err = QString());
            bool showingLoginWithCaptcha(const QString & title, const QUrl & pict_url, QString & login, QString & pass, QString & captcha, const QString & err = QString());
        };
}

#endif // IUSER_INTERACTION
