#ifndef SOUNDCLOUD_API_H
#define SOUNDCLOUD_API_H

#include "../web_api.h"
#include "../auth_chemas/teu_auth.h"
#include "soundcloud_request_api.h"
#include "soundcloud_api_keys.h"

namespace Soundcloud {
    class Api : public WebApi, public TeuAuth, public RequestApi {
        Q_OBJECT
    public:
        static Api * instance();
        static Api * instance(Json::Obj obj);
        inline static void close() { delete self; }

        inline QString name() const { return QStringLiteral("Soundcloud"); }
        inline Playo3::WebSubType siteType() { return Playo3::sc_site; }
        inline QUrlQuery genDefaultParams() { return QUrlQuery(QStringLiteral("client_id=8f84790a84f5a5acd1c92e850b5a91b7")); }
        QString authUrl();

        void fromJson(Json::Obj & hash);
        Json::Obj toJson();

        inline bool isConnected() { return !token().isEmpty(); }

        void getGroupInfo(QString uid, Json::Obj & object);
        void getUserInfo(QString & uid, Json::Obj & object);

        Json::Obj objectInfo(QString & uid);
        inline void objectInfo(QString & uid, Func func) {
            registerAsync(
                QtConcurrent::run(this, &Api::objectInfo, uid), func
            );
        }
    public slots:
        inline void disconnect() { WebApi::disconnect(); setParams(QString(), QString(), QString()); }
        void proceedAuthResponse(const QUrl & url);

    protected:
        inline QString refresh(QString path) { return path; }
        inline QString baseUrlStr(const QString & predicate) { return base_url % predicate % ".json"; }

        inline QString offsetKey() const { return offset_key; }
        inline QString limitKey() const { return limit_key; }
        inline int requestLimit() const { return 200; }

        inline Json::Obj & extractBody(Json::Obj & response) { return response; }
        inline bool endReached(Json::Obj & response, int /*offset*/) { return response.value(QStringLiteral("response")).toArray().isEmpty(); }
        inline bool extractStatus(QUrl & /*url*/, Json::Obj & response, int & code, QString & message) {
            Json::Obj stat_obj = response.obj(QStringLiteral("response")).arr(QStringLiteral("errors"))[0].obj();
            message = stat_obj.str(QStringLiteral("error_message"));
            return (code = stat_obj.num(QStringLiteral("error_code"))) == 0;
        }
    private:
        inline Api(Json::Obj & hash) : WebApi(), TeuAuth() { fromJson(hash); }
        inline Api() : WebApi(), TeuAuth() { }
        inline virtual ~Api() {}

        static Api * self;
    };
}

#endif // SOUNDCLOUD_API_H
