#ifndef OAUTH
#define OAUTH

#include "modules/core/web/utils/web_manager.h"

#define NOUNCE_SET QStringLiteral("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789")

namespace Core {
    namespace Web {
        class OAuth {
            QString consumer_key;
            QString token;

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

            QString hmacSha1(const QString & message, const QString & key) {
                QByteArray keyBytes = key.toLatin1();
                int keyLength = keyBytes.size();              // Lenght of key word
                const int blockSize = 64;   // Both MD5 and SHA-1 have a block size of 64.

                // If key is longer than block size, we need to hash the key
                if (keyLength > blockSize) {
                    QCryptographicHash hash(QCryptographicHash::Sha1);
                    hash.addData(keyBytes);
                    keyBytes = hash.result();
                }

                // Create the opad and ipad for the hash function.
                QByteArray ipad, opad;

                ipad.fill(0, blockSize);
                opad.fill(0, blockSize);

                ipad.replace(0, keyBytes.length(), keyBytes);
                opad.replace(0, keyBytes.length(), keyBytes);

                /* http://tools.ietf.org/html/rfc2104 - (2) & (5) */
                for (int i=0; i<64; i++) {
                    ipad[i] = ipad[i] ^ 0x36;
                    opad[i] = opad[i] ^ 0x5c;
                }

                QByteArray workArray;
                workArray.clear();

                workArray.append(ipad, 64);
                /* http://tools.ietf.org/html/rfc2104 - (3) */
                workArray.append(message.toLatin1());


                /* http://tools.ietf.org/html/rfc2104 - (4) */
                QByteArray sha1 = QCryptographicHash::hash(workArray, QCryptographicHash::Sha1);

                /* http://tools.ietf.org/html/rfc2104 - (6) */
                workArray.clear();
                workArray.append(opad, 64);
                workArray.append(sha1);

                sha1.clear();

                /* http://tools.ietf.org/html/rfc2104 - (7) */
                sha1 = QCryptographicHash::hash(workArray, QCryptographicHash::Sha1);
                return QString(sha1.toBase64());
            }

//            1.  The scheme and host MUST be in lowercase.

//            2.  The host and port values MUST match the content of the HTTP
//                request "Host" header field.
//            3.  The port MUST be included if it is not the default port for the
//                scheme, and MUST be excluded if it is the default.  Specifically,
//                the port MUST be excluded when making an HTTP request [RFC2616]
//                to port 80 or when making an HTTPS request [RFC2818] to port 443.
//                All other non-default port numbers MUST be included.
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

                qDebug() << "BASE_STR" << base_str;

                QString secret = QString(QUrl::toPercentEncoding(consumer_key)) % '&' % QString(QUrl::toPercentEncoding(token));
                QString token = hmacSha1(base_str, secret);
                qDebug() << "TOKEN" << token;

                return token;
            }

        public:
            OAuth(const QString & consumer_key, const QString & token) : consumer_key(consumer_key), token(token) {}

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
                    {QStringLiteral("oauth_consumer_key"), consumer_key},
                    {QStringLiteral("oauth_token"), token},
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
                qDebug() << "SOSO" << resp -> toText();

                return false;
            }

            bool autorize() {
                return false;
            }

            bool access() {
                return false;
            }
        };
    }
}

#endif // OAUTH
