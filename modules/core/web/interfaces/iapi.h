#ifndef IAPI
#define IAPI

#include "settings.h"
#include "iqueriable.h"
#include "dialogs/user_action_dialog.h"

#include "modules/core/interfaces/isearchable.h"
#include "modules/core/misc/format.h"

//#include "misc/web_utils/json.h"

#define DEFAULT_LIMIT_AMOUNT 99999

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

//            void setLimit(QUrlQuery & query, int limit = DEFAULT_LIMIT_AMOUNT, QVariant offset = 0) {
//                if (offset.type() == QVariant::Int) {
//                    int of = offset.toInt();
//                    if (of > 0) setParam(query, offsetKey(), QString::number(of));
//                } else if (offset.type() == QVariant::String) {
//                    QString of = offset.toString();
//                    if (!of.isEmpty()) setParam(query, offsetKey(), of);
//                }
//                setParam(query, limitKey(), QString::number(limit));
//            }

            inline void setParam(QUrlQuery & query, const QString & name, int value) { query.addQueryItem(name, QString::number(value)); }
            inline void setParam(QUrlQuery & query, const QString & name, float value) { query.addQueryItem(name, QString::number(value)); }
            inline void setParam(QUrlQuery & query, const QString & name, const QString & value) { query.addQueryItem(name, value); }
            inline void setParam(QUrlQuery & query, const QString & name, const QStringList & values) {
                if (values.isEmpty()) return;
                for(QStringList::ConstIterator val = values.constBegin(); val != values.constEnd(); val++)
                    query.addQueryItem(name, *val);
            }

//            virtual QUrl buildUrl(QUrl tUrl, int offset, int limit, const QJsonObject & /*prev_response*/) {
//                QUrl url(tUrl);
//                QUrlQuery query = QUrlQuery(url);
//                setLimit(query, limit, offset);
//                url.setQuery(query);
//                return url;
//            }
        };
    }
}

#endif // IAPI
