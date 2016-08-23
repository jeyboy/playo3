#ifndef ISOURCE
#define ISOURCE

#include "icmd.h"
#include "isearchable.h"
#include "isource_auth_perm.h"
#include "isource_feeds.h"
#include "modules/core/misc/thread_utils.h"
#include "modules/core/web/grabber_keys.h"
#include "modules/core/web/interfaces/queriable_response.h"

#define UID_HEAD QStringLiteral("@")
#define ISOURCE_ATTRS_KEY QStringLiteral("attrs")
#define OPERATION_BLOCKED QStringLiteral("block")

namespace Core {
    class ISource : public ISourceAuthPerm, public ISourceFeeds, public ISearchable, public ICmd {
        Q_OBJECT
    public:
        virtual ~ISource() {}

        virtual QString name() const = 0;
        virtual DataSubType sourceType() const = 0;
        virtual QString cookies() const { return QString(); }

        inline QString lastError() const { return error; }

        inline QString uidStr(const QString & tabId) const { return UID_HEAD % name() % tabId; }
        static QString idToStr(const QJsonValue & id) {
            if (id.isString()) return id.toString();
            return QString::number((qint64)id.toDouble());
        }

        void toJson(QJsonObject & hash) {
            QJsonObject root;

            saveAdditionals(root); // sometimes required to change attrs before saving
            root.insert(ISOURCE_ATTRS_KEY, QJsonObject::fromVariantHash(attrs));

            hash.insert(name(), root);
        }
        void fromJson(const QJsonObject & hash) {
            QJsonObject root = hash.value(name()).toObject();

            attrs = root.value(ISOURCE_ATTRS_KEY).toObject().toVariantHash();
            loadAdditionals(root);
        }

        virtual QJsonObject itemsInfo(const QStringList & /*items_uids*/) { return QJsonObject(); }

        virtual inline QString refresh(const QString & refresh_page, const DataMediaType & itemMediaType) {
            if (refresh_page.isEmpty() || !isShareable()) return QString();
            return refreshProc(takeRefreshPage(refresh_page), itemMediaType);
        }

        // proc charts / tops and other ...
        virtual inline QMap<QString, QString> setsList() { return QMap<QString, QString>(); }

        QToolButton * initButton(QWidget * parent = 0);

    public slots:
        void openTab();
        void openRecomendations();
        void openRelationTab();
        void openPackageTab();
    protected:
        QJsonObject prepareBlock(const DataMediaType & dmt_val, const QJsonValue & block_content, const std::initializer_list<std::pair<QString, QString> > & params = {}) {
            QJsonObject res = QJsonObject {
                {Web::tkn_content, block_content}, {Web::tkn_media_type, dmt_val}, {Web::tkn_source_id, sourceType()}
            };

            if (params.size() > 0)
                for (typename std::initializer_list<std::pair<QString, QString> >::const_iterator it = params.begin(); it != params.end(); ++it)
                    res.insert(it -> first, it -> second);

            return res;
        }
        QJsonObject prepareBlock(const DataMediaType & dmt_val, const ICmdMethods & mtd, const Web::QueriableResponse & response, const std::initializer_list<std::pair<QString, QVariant> > & params = {}) {
            QJsonObject block;

            int source_id = sourceType();

            block.insert(Web::tkn_content, response.content);
            block.insert(Web::tkn_media_type, dmt_val);
            block.insert(Web::tkn_source_id, source_id);

            QUrlQuery query = Cmd::paramsToQuery(params);
            query.addQueryItem(CMD_OFFSET, QString::number(response.next_offset));
            query.addQueryItem(CMD_ITEMS_LIMIT, QString::number(response.items_limit));
            query.addQueryItem(CMD_REQUESTS_LIMIT, QString::number(response.requests_limit));

            if (!response.isFinished())
                block.insert(
                    Web::tkn_more_cmd,
                    Cmd::build(source_id, mtd, query).toString()
                );

            return block;
        }
        QJsonObject prepareBlock(const DataMediaType & dmt_val, const ICmdMethods & mtd, const Web::QueriableResponse & response, const SearchLimit & limits, const std::initializer_list<std::pair<QString, QVariant> > & params = {}) {
            QJsonObject block;

            int source_id = sourceType();

            block.insert(Web::tkn_content, response.content);
            block.insert(Web::tkn_media_type, dmt_val);
            block.insert(Web::tkn_source_id, source_id);

            QUrlQuery query = limits.toICmdParams(response.next_offset);
            Cmd::paramsToQuery(query, params);

            if (!response.isFinished())
                block.insert(
                    Web::tkn_more_cmd,
                    Cmd::build(
                        source_id, mtd,
                        query
                    ).toString()
                );

            return block;
        }

        virtual Web::Response * takeRefreshPage(const QString & refresh_page) { return Web::Manager::prepare() -> getFollowed(QUrl(refresh_page)); }
        virtual QString refreshProc(Web::Response * response, const DataMediaType & /*itemMediaType*/) { delete response; return QString(); }
    };
}

#endif // ISOURCE
