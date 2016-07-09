#ifndef ISOURCE
#define ISOURCE

#include "isearchable.h"
#include "isource_auth_perm.h"
#include "isource_feeds.h"
#include "modules/core/data_sub_types.h"
#include "modules/core/misc/thread_utils.h"

#define UID_HEAD QStringLiteral("@")
#define ISOURCE_ATTRS_KEY QStringLiteral("attrs")

namespace Core {
    class ISource : public ISourceAuthPerm, public ISourceFeeds, public ISearchable {
        Q_OBJECT
    public:
        virtual ~ISource() {}

        virtual QString name() const = 0;
        virtual DataSubType siteType() const = 0;

        inline QString lastError() const { return error; }

        inline QString uidStr(const QString & tabId) const { return UID_HEAD % name() % tabId; }

        void toJson(QJsonObject & hash) {
            QJsonObject root;
            root.insert(ISOURCE_ATTRS_KEY, QJsonObject::fromVariantHash(attrs));
            saveAdditionals(root);

            hash.insert(name(), root);
        }
        void fromJson(const QJsonObject & hash) {
            QJsonObject root = hash.value(name()).toObject();

            attrs = root.value(ISOURCE_ATTRS_KEY).toObject().toVariantHash();
            loadAdditionals(root);
        }

        virtual QJsonArray itemsInfo(const QStringList & /*items_uids*/) { return QJsonArray(); }

        virtual inline QString refresh(const QString & refresh_page, const DataMediaType & itemMediaType) {
            if (refresh_page.isEmpty() || !isShareable()) return QString();
            return refreshProc(takeRefreshPage(refresh_page), itemMediaType);
        }
        virtual inline QJsonValue loadSet(const QVariantMap & /*attrs*/) { return QJsonArray(); }

        // proc charts / tops and other ...
        virtual inline QMap<QString, QString> customsList() { return QMap<QString, QString>(); }
        virtual inline QJsonObject procCustom(const QString & /*custom_params*/) { return QJsonObject(); }

        QToolButton * initButton(QWidget * parent = 0);

    public slots:
        void openTab();
        void openRecomendations();
        void openRelationTab();
        void openPackageTab();
    protected:
        virtual Web::Response * takeRefreshPage(const QString & refresh_page) { return Web::Manager::prepare() -> getFollowed(QUrl(refresh_page)); }
        virtual QString refreshProc(Web::Response * response, const DataMediaType & /*itemMediaType*/) { delete response; return QString(); }
    };
}

#endif // ISOURCE
