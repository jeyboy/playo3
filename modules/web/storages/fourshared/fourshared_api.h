#ifndef FOURSHARED_API
#define FOURSHARED_API

#include "../../web_api.h"
#include "../../auth_chemas/teu_auth.h"

namespace Fourshared {
    class Api: public WebApi, public TeuAuth/*, public RequestApi*/ {
        Q_OBJECT
    public:
        static Api * instance();
        static Api * instance(QJsonObject obj);
        inline static void close() { delete self; }

        inline QString name() const { return QStringLiteral("4shared"); }
        inline QUrlQuery genDefaultParams() {
            if (token().isEmpty())
                return QUrlQuery(QStringLiteral("oauth_consumer_key=22abeb63487b7f6b75051079b7e610b1"));
            else
                return QUrlQuery(QStringLiteral("oauth_consumer_key=") % token());
        }
        QString authUrl();

        void fromJson(QJsonObject hash);
        QJsonObject toJson();

        inline bool isConnected() { return !token().isEmpty(); }
//        QString downloadLink();

    public slots:
        inline void disconnect() { WebApi::disconnect(); setParams(QString(), QString(), QString()); }
        void proceedAuthResponse(const QUrl & url);

    protected:
//        inline QString baseUrlStr(QString & predicate) { return base_url % predicate % ".json"; }

        inline QString offsetKey() const { return offset_key; }
        inline QString limitKey() const { return limit_key; }
//        inline int requestLimit() const { return 200; }

//        inline QJsonObject & extractBody(QJsonObject & response) { return response; }
//        inline bool endReached(QJsonObject & response, int /*offset*/) { return response.value(QStringLiteral("response")).toArray().isEmpty(); }
//        inline bool extractStatus(QUrl & /*url*/, QJsonObject & response, int & code, QString & message) {
//            QJsonObject stat_obj = response.value(QStringLiteral("response")).toObject().value(QStringLiteral("errors")).toArray().first().toObject();
//            message = stat_obj.value(QStringLiteral("error_message")).toString();
//            return (code = stat_obj.value(QStringLiteral("error_code")).toInt()) == 0;
//        }
    private:
        inline Api(QJsonObject hash) : WebApi(), TeuAuth() { fromJson(hash); }
        inline Api() : WebApi(), TeuAuth() { }
        inline virtual ~Api() {}

        static Api * self;
    };
}

#endif // FOURSHARED_API
