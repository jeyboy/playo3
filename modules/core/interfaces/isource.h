#ifndef ISOURCE
#define ISOURCE

// https://www.iso.org/obp/ui/#search

#include "icmd.h"
#include "isearchable.h"
#include "isource_auth_perm.h"
#include "isource_feeds.h"

#include "modules/core/defines.h"
#include "modules/core/misc/thread_utils.h"
#include "modules/core/web/grabber_keys.h"
#include "modules/core/web/interfaces/queriable_response.h"

#define TITLE_PARAMS_SPLITTER LSTR(" |?| ")
#define UID_HEAD LSTR("@")
#define ISOURCE_ATTRS_KEY LSTR("attrs")
#define OPERATION_BLOCKED LSTR("block")

#define ID_TOKEN(token1, token2, sep) token1 % sep % token2

namespace Core {
    class ISource : public ISourceAuthPerm, public ISourceFeeds, public ISearchable, public ICmd {
        Q_OBJECT
    public:
        inline bool stub() { return true; }
        virtual ~ISource() {}

        virtual QString name() const = 0;
        virtual DataSubType sourceType() const = 0;
        virtual QString cookies() const { return QString(); }

        inline QString lastError() const { return error; }

        inline QString uidStr(const QString & tabId) const { return UID_HEAD % name() % tabId; }

        void toJson(QJsonObject & hash) {
            QJsonObject root;

            saveAdditionals(root); //INFO: sometimes required to change attrs before saving
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

        QWidget * settingsBlock(int block_type, const QVariantMap & configs);
        void applySettings(QVariantMap & configs);

        virtual void userInfoAsync(const QString & /*user_id*/, Func * /*func*/) {}
        virtual void openSetAsync(const QString & /*set_params*/, Func * /*func*/) {}
        virtual void tagsAudioRecommendationsAsync(const QString & /*track_id*/, Func * /*func*/) {}
        virtual void tagsVideoRecommendationsAsync(const QString & /*track_id*/, Func * /*func*/) {}
        virtual void audioRecommendationsAsync(const QString & /*track_id*/, Func * /*func*/) {}
        virtual void videoRecommendationsAsync(const QString & /*track_id*/, Func * /*func*/) {}
        virtual void userAudioRecommendationsAsync(const QString & /*user_id*/, Func * /*func*/) {}
        virtual void userVideoRecommendationsAsync(const QString & /*user_id*/, Func * /*func*/) {}
        virtual void artistRecommendationsAsync(const QString & /*artist_id*/, Func * /*func*/) {}
        virtual void audioByArtistAsync(const QString & /*artist_id*/, Func * /*func*/) {}
        virtual void videoByArtistAsync(const QString & /*artist_id*/, Func * /*func*/) {}
        virtual void albumByArtistAsync(const QString & /*artist_id*/, Func * /*func*/) {}
        virtual void audioByLabelAsync(const QString & /*label_id*/, Func * /*func*/) {}
        virtual void albumByLabelAsync(const QString & /*label_id*/, Func * /*func*/) {}
        virtual void artistByLabelAsync(const QString & /*label_id*/, Func * /*func*/) {}
    public slots:
        void openTab();
        void openAudioRecs();
        void openVideoRecs();
        void openRelationTab();
        void openPackageTab();
    protected:
        enum SourceSettngsType : int {
            sst_none = 0,
            sst_source,
            sst_feeds,
            sst_stream
        };

        QHash<int, QHash<QString, QWidget *> > settings_forms;

        virtual QWidget * streamSettingsBlock(const QVariant & /*data*/) { return 0; }
        virtual QWidget * sourceSettingsBlock(const QVariant & /*data*/) { return 0; }
        virtual QWidget * feedsSettingsBlock(const QVariant & /*data*/) { return 0; }

        virtual void applyStreamSettings(const QHash<QString, QWidget *> & /*data*/, QVariant & /*config*/) { }
        virtual void applySourceSettings(const QHash<QString, QWidget *> & /*data*/, QVariant & /*config*/) { }
        virtual void applyFeedsSettings(const QHash<QString, QWidget *> & /*data*/, QVariant & /*config*/) { }

        QJsonObject prepareBlock(const DataMediaType & dmt_val, const QJsonValue & block_content,
            const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
            QJsonObject res = QJsonObject {
                {Web::tkn_content, block_content}, {Web::tkn_media_type, dmt_val}, {Web::tkn_source_id, sourceType()},
                {Web::tkn_is_empty, block_content.isArray() ? block_content.toArray().isEmpty() : block_content.toObject().isEmpty()}
            };

            if (block_params.size() > 0)
                for (typename std::initializer_list<std::pair<QString, QString> >::const_iterator it = block_params.begin(); it != block_params.end(); ++it)
                    res.insert(it -> first, it -> second);

            return res;
        }
        QJsonObject prepareBlock(const DataMediaType & dmt_val, const ICmdMethods & mtd, const Web::QueriableResponse & response, const std::initializer_list<std::pair<QString, QString> > & block_params = {}, const std::initializer_list<std::pair<QString, QVariant> > & cmd_params = {}) {
            QJsonObject block;

            int source_id = sourceType();

            block.insert(Web::tkn_content, response.content);
            block.insert(Web::tkn_media_type, dmt_val);
            block.insert(Web::tkn_source_id, source_id);
            block.insert(Web::tkn_is_empty, response.content.isEmpty());

            if (block_params.size() > 0)
                for (typename std::initializer_list<std::pair<QString, QString> >::const_iterator it = block_params.begin(); it != block_params.end(); ++it)
                    block.insert(it -> first, it -> second);

            if (!response.finished) {
                QUrlQuery query = Cmd::paramsToQuery(cmd_params);
                query.addQueryItem(CMD_OFFSET, response.next_offset);
                query.addQueryItem(CMD_ITEMS_LIMIT, QString::number(response.items_limit));
                query.addQueryItem(CMD_REQUESTS_LIMIT, QString::number(response.requests_limit));

                block.insert(
                    Web::tkn_more_cmd,
                    Cmd::build(source_id, mtd, query).toString()
                );
            }

            return block;
        }
        QJsonObject prepareBlock(const DataMediaType & dmt_val, const ICmdMethods & mtd, const Web::QueriableResponse & response, const SearchLimit & limits,
                const std::initializer_list<std::pair<QString, QString> > & block_params = {}, const std::initializer_list<std::pair<QString, QVariant> > & cmd_params = {}) {
            QJsonObject block;

            int source_id = sourceType();

            block.insert(Web::tkn_content, response.content);
            block.insert(Web::tkn_media_type, dmt_val);
            block.insert(Web::tkn_source_id, source_id);
            block.insert(Web::tkn_is_empty, response.content.isEmpty());

            if (block_params.size() > 0)
                for (typename std::initializer_list<std::pair<QString, QString> >::const_iterator it = block_params.begin(); it != block_params.end(); ++it)
                    block.insert(it -> first, it -> second);

            if (!response.finished) {
                QUrlQuery query = limits.toICmdParams(response.next_offset);
                Cmd::paramsToQuery(query, cmd_params);

                block.insert(
                    Web::tkn_more_cmd,
                    Cmd::build(
                        source_id, mtd,
                        query
                    ).toString()
                );
            }

            return block;
        }

        virtual Web::Response * takeRefreshPage(const QString & refresh_page) { return Web::Manager::prepare() -> getFollowed(QUrl(refresh_page)); }
        virtual QString refreshProc(Web::Response * response, const DataMediaType & /*itemMediaType*/) { delete response; return QString(); }
    };
}

#endif // ISOURCE
