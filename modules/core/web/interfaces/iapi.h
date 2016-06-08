#ifndef IAPI
#define IAPI

#include "settings.h"
#include "iqueriable.h"
#include "dialogs/user_action_dialog.h"

#include "modules/core/interfaces/isearchable.h"
#include "modules/core/misc/format.h"

//#include "misc/web_utils/json.h"

#define MAX_PAGES_PER_ARTIST 2

namespace Core {
    namespace Web {
        class IApi : public IQueriable, public ISearchable {
        public:
            inline IApi() { actionDialog = new UserActionDialog(Settings::obj().anchorWidget()); }
            inline virtual ~IApi() {}
        protected:
            UserActionDialog * actionDialog;

            bool showingCaptcha(const QUrl & pict_url, QString & result);
            bool showingLogin(const QString & title, QString & login, QString & pass, const QString & err = QString());
            bool showingLoginWithCaptcha(const QString & title, const QUrl & pict_url, QString & login, QString & pass, QString & captcha, const QString & err = QString());

            inline void setParam(QUrlQuery & query, const QString & name, int value) {
                if (value == IGNORE_PARAM) return;
                query.addQueryItem(name, QString::number(value));
            }
            inline void setParam(QUrlQuery & query, const QString & name, float value) {
                if (value == IGNORE_PARAM) return;
                query.addQueryItem(name, QString::number(value));
            }
            inline void setParam(QUrlQuery & query, const QString & name, const QString & value) { query.addQueryItem(name, value); }
            inline void setParam(QUrlQuery & query, const QString & name, const QStringList & values, bool as_one_key = true, char join_symb = ',') {
                if (values.isEmpty()) return;

                if (as_one_key)
                    setParam(query, name, values.join(join_symb));
                else
                    for(QStringList::ConstIterator val = values.constBegin(); val != values.constEnd(); val++)
                        query.addQueryItem(name, *val);
            }
        };
    }
}

#endif // IAPI
