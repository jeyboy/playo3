#ifndef OD_API_H
#define OD_API_H

#include "../web_api.h"
#include "../auth_chemas/teu_auth.h"
#include "../iapi.h"
#include "misc/web_utils/html_parser.h"

namespace Od {
    class Api : public WebApi, public TeuAuth, public IApi {
        Q_OBJECT
    public:
        static Api * instance();
        static Api * instance(QJsonObject obj);
        inline static void close() { delete self; }

        inline QString name() const { return QStringLiteral("Od"); }
        inline Playo3::WebSubType siteType() { return Playo3::od_site; }
        inline QUrlQuery genDefaultParams() { return QUrlQuery(/*QStringLiteral("client_id=8f84790a84f5a5acd1c92e850b5a91b7")*/); }
        QString authUrl();

        void fromJson(QJsonObject hash);
        QJsonObject toJson();

        inline bool isConnected() { return !token().isEmpty(); }

//        void getGroupInfo(QString uid, QJsonObject & object);
//        void getUserInfo(QString & uid, QJsonObject & object);

//        QJsonObject objectInfo(QString & uid);
//        inline void objectInfo(QString & uid, Func func) {
//            registerAsync(
//                QtConcurrent::run(this, &Api::objectInfo, uid), func
//            );
//        }
    public slots:
        void connection() {
            QUrl url("https://www.ok.ru/https?st.redirect=&st.asr=&st.posted=set&st.originalaction=http://ok.ru/dk?cmd=AnonymLogin&amp;st.cmd=anonymLogin&st.fJS=on&st.st.screenSize=1920x1080&st.st.browserSize=621&st.st.flashVer=18.0.0&st.email=jeyboy%40bigmir.net&st.password=jeyboy12201985&st.remember=on&st.iscode=false");
            QNetworkReply * reply = WebManager::manager() -> postForm(url);
            Html::Document doc(reply);
            doc.output();
            Html::Set results = doc.find("a.u-menu_a.tdn[href='/profile']");
            QString uid = results.link().section('/', 2);
            delete reply;
            setParams(WebManager::cookie(QStringLiteral("JSESSIONID")), uid, QString());
            qDebug() << token() << userID();
        }
        inline void disconnect() { WebApi::disconnect(); setParams(QString(), QString(), QString()); }
        void proceedAuthResponse(const QUrl & url);

    protected:
        QJsonArray search_postprocess(QString & /*predicate*/, QString & /*genre*/, const SearchLimit & /*limitations*/) { return QJsonArray(); }

        inline QString refresh(QString path) { return path; }
        inline QString baseUrlStr(const QString & predicate) { /*return base_url % predicate % ".json";*/ }

        inline QString offsetKey() const { /*return offset_key;*/ }
        inline QString limitKey() const { /*return limit_key;*/ }
        inline int requestLimit() const { return 100; }

        inline QJsonObject & extractBody(QJsonObject & response) { return response; }
        inline bool endReached(QJsonObject & response, int /*offset*/) { return response.value(QStringLiteral("response")).toArray().isEmpty(); }
        inline bool extractStatus(QUrl & /*url*/, QJsonObject & /*response*/, int & /*code*/, QString & /*message*/) {
//            QJsonObject stat_obj = response.value(QStringLiteral("response")).toObject().value(QStringLiteral("errors")).toArray().first().toObject();
//            message = stat_obj.value(QStringLiteral("error_message")).toString();
            return true/*(code = stat_obj.value(QStringLiteral("error_code")).toInt()) == 0*/;
        }
    private:
        inline Api(QJsonObject hash) : WebApi(), TeuAuth() { fromJson(hash); }
        inline Api() : WebApi(), TeuAuth() { }
        inline virtual ~Api() {}

        static Api * self;
    };
}

#endif // SOUNDCLOUD_API_H
