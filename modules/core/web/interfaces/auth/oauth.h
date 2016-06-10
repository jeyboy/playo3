#ifndef OAUTH
#define OAUTH

#include "modules/core/web/utils/web_manager.h"
#include <QMessageAuthenticationCode>

#define NOUNCE_SET QStringLiteral("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")

namespace Core {
    namespace Web {
        class OAuth {
            QString consumer_key;
            QString consumer_secret;

            QString oauth_token;
            QString oauth_token_secret;

            QString nonce(int length, quint64 seed) {
                qsrand(seed);
                const QString possibleCharacters = NOUNCE_SET;
                int valid_range = possibleCharacters.length();

                QString randomString;
                for(int i = 0; i < length; ++i) {
                    int index = qrand() % valid_range;
                    randomString.append(possibleCharacters.at(index));
                }
                return randomString;
            }

            QString signature(const QString & html_method, const QUrl & uri, const QMap<QString, QString> & attrs) {
                QString base_uri_path = uri.path();

                int opts = QUrl::RemovePath | QUrl::RemoveQuery | QUrl::RemoveFragment;
                int port = uri.port();

                if (port != -1) {
                    QString scheme = uri.scheme().toLower();

                    if ((scheme == QStringLiteral("http") && port == 80) ||
                        (scheme == QStringLiteral("https") && port == 443))
                        opts |= QUrl::RemovePort;
                }

                QString base_uri = uri.toDisplayString((QUrl::UrlFormattingOption)opts).toLower();

                QString base_params;
                for(QMap<QString, QString>::ConstIterator it = attrs.cbegin(); it != attrs.cend(); it++)
                    base_params = base_params % QStringLiteral("&%1=%2").arg(QUrl::toPercentEncoding(it.key()), it.value());


                QString base_str = QStringLiteral("%1&%2%3&%4").arg(
                    html_method.toUpper(),
                    QUrl::toPercentEncoding(base_uri),
                    QUrl::toPercentEncoding(base_uri_path),
                    QUrl::toPercentEncoding(base_params.mid(1))
                );

//                qDebug() << "BASE_STR" << base_str;

                QString secret = QString(QUrl::toPercentEncoding(consumer_secret)) % '&' % QString(QUrl::toPercentEncoding(oauth_token_secret));
//                qDebug() << "SECRET" << secret;

                QByteArray res = QMessageAuthenticationCode::hash(base_str.toUtf8(), secret.toUtf8(), QCryptographicHash::Sha1);

                qDebug() << "RES" << res.toBase64();

                return res.toBase64();
            }

        public:
            OAuth(const QString & consumer_key, const QString & consumer_secret) : consumer_key(consumer_key), consumer_secret(consumer_secret) {}

            bool initiateGet(const QString & url) { // not tested
                QUrl uri(url);

                quint64 timestamp = QDateTime::currentMSecsSinceEpoch();

                QMap<QString, QString> attrs = {
                    {QStringLiteral("oauth_consumer_key"), consumer_key},
//                    {QStringLiteral("oauth_token"), token},
                    {QStringLiteral("oauth_signature_method"), QStringLiteral("HMAC-SHA1")},
                    {QStringLiteral("oauth_timestamp"), QString::number(timestamp)},
                    {QStringLiteral("oauth_nonce"), nonce(8, timestamp)},
                };

                QString oauth_header = QString(
                    "OAuth realm=\"%1\","
                    "oauth_signature=\"%2\","
                    "oauth_version=\"1.0\""
                );
//                oauth_callback="http%3A%2F%2Fprinter.example.com%2Fready",

                for(QMap<QString, QString>::Iterator it = attrs.begin(); it != attrs.end(); it++)
                    oauth_header = oauth_header % QStringLiteral(",%1=%2").arg(it.key(), it.value());

                QUrlQuery query(uri.query());

                QList<QPair<QString, QString> > items = query.queryItems();

                for(QList<QPair<QString, QString> >::Iterator it = items.begin(); it != items.end(); it++)
                    attrs.insert(QUrl::toPercentEncoding((*it).first), QUrl::toPercentEncoding((*it).second));

                oauth_header = oauth_header.arg(
                    uri.host(),
                    signature(QStringLiteral("GET"), uri, attrs)
                );

                QHash<QString, QString> headers;
                headers.insert(QStringLiteral("Authorization"), oauth_header);

                Response * resp = Manager::prepare() -> followedPost(url, headers);
                qDebug() << "SOSO" << resp -> toText();

                return false;
            }

            bool initiatePost(const QString & url) {
                QUrl uri(url);

                quint64 timestamp = QDateTime::currentMSecsSinceEpoch() / 1000;

                QMap<QString, QString> attrs = {
                    {QStringLiteral("oauth_version"), QStringLiteral("1.0")},
                    {QStringLiteral("oauth_consumer_key"), consumer_key},
                    {QStringLiteral("oauth_signature_method"), QStringLiteral("HMAC-SHA1")},
                    {QStringLiteral("oauth_timestamp"), QString::number(timestamp)},
                    {QStringLiteral("oauth_nonce"), nonce(8, timestamp)},
                };

                QUrlQuery query(uri.query());

                QList<QPair<QString, QString> > items = query.queryItems();
                for(QList<QPair<QString, QString> >::Iterator it = items.begin(); it != items.end(); it++)
                    attrs.insert(QUrl::toPercentEncoding((*it).first), QUrl::toPercentEncoding((*it).second));

                attrs.insert(QStringLiteral("oauth_signature"), signature(QStringLiteral("POST"), uri, attrs));

                QUrlQuery new_query;

                for(QMap<QString, QString>::ConstIterator it = attrs.cbegin(); it != attrs.cend(); it++)
                    new_query.addQueryItem(it.key(), it.value());

                uri.setQuery(new_query);

                Response * resp = Manager::prepare() -> followedForm(uri);
                QUrlQuery result_params = resp -> toQuery();
                qDebug() << "SOSO" << result_params.toString();

                if (result_params.hasQueryItem(QStringLiteral("oauth_token"))) {
                    oauth_token = result_params.queryItemValue(QStringLiteral("oauth_token"));
                    oauth_token_secret = result_params.queryItemValue(QStringLiteral("oauth_token_secret"));
//                    oauth_callback_confirmed=true

                    return true;
                }

                return false;
            }

            bool autorize(const QString & url) {
//                https://api.4shared.com/v1_2/oauth/authorize?oauth_token=42cac70e531abd3ac1152bd7bfcb58a2&oauth_callback=http%3A%2F%2Fterm.ie%2Foauth%2Fexample%2Fclient.php%3Fkey%3D22abeb63487b7f6b75051079b7e610b1%26secret%3D71970e08961f3a78e821f51f989e6cb568cbd0ce%26token%3D42cac70e531abd3ac1152bd7bfcb58a2%26token_secret%3De11ef020926484b3a2ef46eff509706808cc2ab8%26endpoint%3Dhttps%253A%252F%252Fapi.4shared.com%252Fv1_2%252Foauth%252Fauthorize

                return false;
            }

            bool access(const QString & url) {
                return false;
            }
        };
    }
}

#endif // OAUTH
