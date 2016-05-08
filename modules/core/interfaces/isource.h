#ifndef ISOURCE
#define ISOURCE

#include <qstring.h>
#include <qtoolbutton.h>

#include "modules/core/web/utils/web_manager.h"
#include "modules/core/web/interfaces/iqueriable.h"
#include "modules/core/data_sub_types.h"
#include "isearchable.h"

#define UID_HEAD QStringLiteral("@")

namespace Core {
    enum ConnectionType : int {
        connection_restore = 1,
        connection_oauth = 2,
        connection_manual = 4
    };

    namespace Web { class Sociable; }

    class ISource : public Web::IQueriable, public ISearchable {
    public:
        ISource() : button(0) {}

        virtual QString name() const = 0;
        virtual DataSubType siteType() const = 0;

        inline QString lastError() const { return error; }

        virtual inline bool isConnected() { return true; }
        virtual inline bool connectUser(const ConnectionType & /*conType*/ = connection_restore) { return false; }
        virtual inline void disconnectUser() { }

        inline QString uidStr(const QString & tabId) const { return UID_HEAD % name() % tabId; }

        virtual void toJson(QJsonObject & /*hash*/) { qDebug() << name() << "TO JSON"; } // stub
        virtual void fromJson(const QJsonObject & /*hash*/) { qDebug() << name() << "FROM JSON"; } // stub

        virtual inline QString refresh(const QString & refresh_page) {
            if (refresh_page.isEmpty()) return QString();
            return refresh_process(take_refresh_page(refresh_page));
        }

        virtual QToolButton * initButton(QWidget * /*parent*/ = 0) { return button; }

        void openTab(const QString & userID);
        void openRecomendations(const QString & userID);
        void openRelationTab(Web::Sociable * currApi);
    protected:
        QToolButton * button;

        virtual Web::Response * take_refresh_page(const QString & refresh_page) { return Web::Manager::prepare() -> followedGet(QUrl(refresh_page)); }
        virtual QString refresh_process(Web::Response * response) { delete response; return QString(); }

        QString error;
    };
}

#endif // ISOURCE
