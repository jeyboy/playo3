#ifndef OD_API_H
#define OD_API_H

#include "../web_api.h"
#include "../auth_chemas/teu_auth.h"
#include "../iapi.h"

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

        void connecting() {
//            POST https://www.ok.ru/https HTTP/1.1
//            Host: www.ok.ru
//            User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0
//            Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
//            Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3
//            Accept-Encoding: gzip, deflate
//            Referer: http://ok.ru/
//            Cookie: bci=-7810169236141871581; _flashVersion=18
//            Connection: keep-alive
//            Content-Type: application/x-www-form-urlencoded
//            Content-Length: 289

//            st.redirect=&st.asr=&st.posted=set&st.originalaction=http%3A%2F%2Fok.ru%2Fdk%3Fcmd%3DAnonymLogin%26st.cmd%3DanonymLogin&st.fJS=on&st.st.screenSize=1920+x+1080&st.st.browserSize=969&st.st.flashVer=18.0.0&st.email=jeyboy%40bigmir.net&st.password=jeyboy12201985&st.remember=on&st.iscode=false

//            ------------------------------------------------------------------------

//            GET http://ok.ru/dk?cmd=AnonymLogin&st.cmd=anonymLogin&httpsdata=Umlm81sLBth1zoe4Gvr91su9jMGy8-9YHxVHKKT2mVev577x2yILuVz1rAETfg1kKu5H6kkjmX1umDYLjK0X6t9FFtKWE8FbHqjd3DFIZp9ZcPRGsRTamryfuTHAbFpoa8-fzj08H0XtkftqWJQt-2J6QNHyMPdYyiIzeoMjGupkLxdRFYTvDS6xUjZQRF9WdVe7Cb7_yNyuOThSK775Z6wwK5yrEN-cF8yfzugRquI6oAUberHcry2T_nuc9w2m HTTP/1.1
//            Host: ok.ru
//            User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0
//            Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
//            Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3
//            Accept-Encoding: gzip, deflate
//            Referer: http://ok.ru/
//            Cookie: bci=-7810169236141871581; _flashVersion=18; viewport=1040; BANNER_LANG=ru; TZD=6.-639; _fo4cl=1
//            Connection: keep-alive

//            ------------------------------------------------------------------------

//            GET http://ok.ru/ HTTP/1.1
//            Host: ok.ru
//            User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0
//            Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
//            Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3
//            Accept-Encoding: gzip, deflate
//            Referer: http://ok.ru/
//            Cookie: bci=-7810169236141871581; _flashVersion=18; viewport=1040; BANNER_LANG=ru; TZD=6.-639; _fo4cl=1; AUTHCODE=X__rkHgxkeJ9R2RmnUvCj-uV-TSeg9cYH34HE4FzzVyiLD_1ku28GkefctAGNkNwDpgxfs60bYovXOe5GpOkP0id58kLIBf356XNq1YlbL0_2; JSESSIONID=b8eb445ba1fd64389de38807882b46d18f59e1f03592f69c.302c0adb; LASTSRV=ok.ru; bd_login_cookie="1440804977439:3704f64905fd1cf360be1acb26bf8b88"
//            Connection: keep-alive


            QUrl url("https://www.ok.ru/https?st.redirect=&st.asr=&st.posted=set&st.originalaction=http%3A%.cmd%3DanonymLogin&st.fJS=on&st.st.screenSize=1920+x+1080&st.st.browserSize=621&st.st.flashVer=18.0.0&st.email=jeyboy%40bigmir.net&st.password=jeyboy12201985&st.remember=on&st.iscode=false");
            QNetworkRequest request(url);
            request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/x-www-form-urlencoded"));
            request.setRawHeader("Referer", "http://ok.ru/");
            request.setRawHeader("Host", "http://ok.ru/");
            request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows NT 6.1; WOW64; rv:40.0) Gecko/20100101 Firefox/40.0");
            request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
            request.setRawHeader("Cookie", "bci=-7810169236141871581; _flashVersion=18");

            QNetworkReply * res = WebManager::manager() -> proceedReply(WebManager::manager() -> postSync(request, url.query(QUrl::FullyEncoded).toUtf8()));
            qDebug() << res -> readAll();

        }

//        void getGroupInfo(QString uid, QJsonObject & object);
//        void getUserInfo(QString & uid, QJsonObject & object);

//        QJsonObject objectInfo(QString & uid);
//        inline void objectInfo(QString & uid, Func func) {
//            registerAsync(
//                QtConcurrent::run(this, &Api::objectInfo, uid), func
//            );
//        }
    public slots:
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
