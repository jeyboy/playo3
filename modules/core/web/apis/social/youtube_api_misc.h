#ifndef YOUTUBE_API_MISC
#define YOUTUBE_API_MISC

#include "modules/core/web/interfaces/iapi.h"
#include "modules/core/web/utils/js_parser.h"
#include "youtube_api_keys.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            class Misc : public IApi {
                struct JsMethod {
                    QString name;
                    QString code;
                };

                struct FmtOption {
                    FmtOption() : quality_id(0), audio(false), adaptive(false), decode_required(false) {}

                    QString toUrl() {
                        if (decode_required)
                            return url % QStringLiteral("&signature=") % signature;

                        return url;
                    }

                    int quality_id;
                    bool audio;
                    bool adaptive;
                    bool decode_required;
                    QString url;
                    QString signature;
                };

                // cpn
                //                var l11 = ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "-", "_"];
                //                var l1I = "";
                //                var lIl = 0;
                //                while (lIl < 16) {
                //                l1I = (l1I + l11[Math.floor((Math.random() * 64))]);
                //                lIl++;
                //                }
                //                l1I;

                QString extractFromPage(const QString & id, QHash<int, FmtOption> & options) {
                    //there should be 2 variants:
                        //  str has 's' param, which should be decoded first with usage of separate js file
                            // http://stackoverflow.com/questions/21510857/best-approach-to-decode-youtube-cipher-signature-using-php-or-js
                            // http://www.codeproject.com/Articles/2352/JavaScript-call-from-C
                            // https://github.com/bitnol/CipherAPI

                    QString response = Web::Manager::prepare() -> followedGet(url_embed.arg(id)) -> toText();
                    int pos = response.indexOf(tkn_url_encoded_fmt_stream_map);
                    if (pos != -1) {
                        pos += tkn_url_encoded_fmt_stream_map.length() + 3;
                        int len = response.indexOf('"', pos) - pos;
                        QStringList templates = response.mid(pos, len).split(',');
                        proceedLinkTemplates(templates, options, QStringLiteral("\\u0026"));

                        // need to split by \\u0026
//                        QString res = links.first().replace("\\u0026", "&");
//                        qDebug() << "LINK!" << res;
//                        qDebug() << "------------------------------";
//                        QUrlQuery vQuery(res);
//                        res = QUrl::fromPercentEncoding(vQuery.queryItemValue(QStringLiteral("url")).toLatin1()) + "&cpn=Twcgu-mcw4SjsEIM";
//                        qDebug() << "LINK" << res << vQuery.toString();
//                        return res;
                    }

                    pos = response.indexOf(tkn_adaptive_fmts);
                    if (pos != -1) {
                        pos += tkn_adaptive_fmts.length() + 3;
                        int len = response.indexOf('"', pos) - pos;
                        QStringList templates = response.mid(pos, len).split(',');
                        proceedLinkTemplates(templates, options, QStringLiteral("\\u0026"));
                    }
                }


                bool extractJsUrl(const QString & html, QString & jsUrl) {
                    QRegExp expression = QRegExp("html5player.+\\.js");
                    expression.setMinimal(true);
                    if (expression.indexIn(html) != -1) {
                        jsUrl = "http://s.ytimg.com/yts/jsbin/" + expression.cap(0).replace("\\/", "/");
                        return true;
                    }

                    expression = QRegExp("//[^\"]*html5player[^\"]*\\.js");
                    expression.setMinimal(true);
                    if (expression.indexIn(html) != -1) {
                        jsUrl = "https:" + expression.cap(0);
                        return true;
                    }


                    expression = QRegExp("//[^\"]*player[^\"]*\\/base\\.js");
                    expression.setMinimal(true);
                    if (expression.indexIn(html) != -1) {
                        jsUrl = "https:" + expression.cap(0);
                        return true;
                    }

                    return false;
                }

                void extractJSMethod(const QString & name, const QString & js, QList<JsMethod> & jsMethods) {
                    for (QList<JsMethod>::Iterator meth = jsMethods.begin(); meth != jsMethods.end(); meth++)
                        if ((*meth).name == name)
                            return;

                    QString escapedName = name;
                    /*escapedName = */escapedName.replace("$", "\\$");
                    QRegExp expression("((?:function\\s*" + escapedName + "|(var\\s*|,\\s*)" + escapedName + "\\s*=(?:\\s*function)?)\\s*(\\([\\w,\\s]*\\))?\\s*)(\\{.*\\})");
                    expression.setCaseSensitivity(Qt::CaseSensitive);
                    if (expression.indexIn((js)) != -1) {
                        JsMethod method;
                        method.name = name;
                        QString descriptor = expression.cap(1);
                        if (!expression.cap(2).isEmpty()) {
                            descriptor = descriptor.right(descriptor.length() - expression.cap(2).length());
                        }
                        QString code = expression.cap(4);
                        QString parameters = expression.cap(3).replace(QRegExp("[\\(\\)\\s]"), "");

                        for (qint64 i = 1; i < code.length(); i++) {
                            QString partial = code.left(i);
                            if (partial.count("{") == partial.count("}")) {
                                method.code = descriptor +  partial;
                                break;
                            }
                        }
                        if (!method.code.isEmpty()) {
                            jsMethods.append(method);

                            expression = QRegExp("([\\w\\$]+)(?:[\\w\\.\\$])*\\s*(\\([\\w\\s,\"\\$]*)\\)");
                            int pos = expression.indexIn(method.code);
                            QStringList expressions;
                            expressions << "function" << "return" << "if" << "elseif";
                            expressions.append(parameters.split(','));

                            while (pos != -1) {
                                if (expressions.indexOf(expression.cap(1)) == -1)
                                    extractJSMethod(expression.cap(1), js, jsMethods);
                                pos = expression.indexIn(method.code, pos + expression.cap(0).length());
                            }
                        }
                    }
                }


                void convertSToSignature(const QString & js, QString & s) {
                    QString methodName;

                    QRegExp expression("signature=|set\\(\"signature\",(.+)\\("); // \\.sig\\|\\|([a-zA-Z0-9$]+)\\(
                    expression.setMinimal(true);
                    if (expression.indexIn(js) != -1) {
                        methodName = expression.cap(1);
                        QList<JsMethod> jsMethods;
                        extractJSMethod(methodName, js, jsMethods);

                        QString resJs;
                        for (QList<JsMethod>::Iterator meth = jsMethods.begin(); meth != jsMethods.end(); meth++)
                            resJs.append((*meth).code).append(";");

                        s = Js::Document::proceedJsCall(resJs, methodName, s);
                    }
                }

//                void convertSignature(const QString & js, const QString & methodName, QString & s) {
//                    qDebug() << "SSSSSSSSSSSSSSSSSSS" << s;
//                    s = Js::Document::proceedJsCall(js, methodName, s);
//                    qDebug() << "WWWWWWWWWWWWWWWWWWW" << s;

////                    qDebug() << "received signature: " << s;

////                    QWebView* view = new QWebView();
////                    view->setHtml("<script>" + this->js + "</script>");
////                    s = view->page()->mainFrame()->evaluateJavaScript(this->signatureMethodName + "(\"" + s + "\")").toString();
////                    qDebug() << "parsed signature: " << s;
////                    view->deleteLater();
//                }

            protected:
                FmtOption chooseFmtByQuality(QHash<int, FmtOption> options) {
                    // update me :)
                    return options.values().first();
                }

                void prepareUrl(QString /*url*/) {
                    //                    !url.contains("ratebypass"))
                    //                            {
                    //                                url = url.append("&ratebypass=yes");
                    //                            }

                }

                void proceedLinkTemplates(const QStringList & templates, QHash<int, FmtOption> & options, const QString & splitter = QString()) {
                    for(QStringList::ConstIterator item = templates.cbegin(); item != templates.cend(); item++) {
                        qDebug() << (*item);
                        FmtOption opt;

                        if (!splitter.isEmpty()) {
                            QStringList parts = (*item).split(splitter); // not used: fallback_host, quality, (for adaptive only): projection_type, init, index, fps, lmt, quality_label, bitrate, size, clen
                            for(QStringList::Iterator part = parts.begin(); part != parts.end(); part++) {
                                qDebug() << (*part);
                                if ((*part).startsWith("url="))
                                    opt.url = decodeStr((*part).mid(4));
                                else if ((*part).startsWith("sig=")) //  str has 'signature' or 'sig' which simply copied to link
                                    opt.signature = (*part).mid(4);
                                else if ((*part).startsWith("signature="))
                                    opt.signature = (*part).mid(10);
                                else if ((*part).startsWith("s=")) {
                                    opt.signature = (*part).mid(2);
                                    opt.decode_required = true;
                                }
                                else if ((*part).startsWith("itag="))
                                    opt.quality_id = (*part).mid(5).toInt();
                                else if ((*part).startsWith("type=")) // each link contains param type equal to the 'video' or 'audio'
                                    opt.audio = (*part).mid(5, 5) == QStringLiteral("audio");
                            }
                        } else {

                        }

                        options.insert(opt.quality_id, opt);
                    }


//                    .replace("\\u0026", "&")
                }

                QString idToUrl(const QString & id) {
                    QHash<int, FmtOption> options;

                    QString response = Web::Manager::prepare() -> followedGet(url_info.arg(id)) -> toText();
                    QUrlQuery query(response);

                    // https://www.quora.com/How-can-I-make-a-YouTube-video-downloader-web-application-from-scratch
                    // https://github.com/rg3/youtube-dl/blob/9dd8e46a2d0860421b4bb4f616f05e5ebd686380/youtube_dl/extractor/youtube.py
                    // http://superuser.com/questions/773719/how-do-all-of-these-save-video-from-youtube-services-work

                    if (query.hasQueryItem(tkn_url_encoded_fmt_stream_map)) {
                        QStringList templates =
                            decodeStr(
                                query.queryItemValue(tkn_url_encoded_fmt_stream_map)
                            ).split(',');

                        proceedLinkTemplates(templates, options, "&");
                    }

                    if (query.hasQueryItem(tkn_adaptive_fmts)) {
                        Logger::obj().write(QStringLiteral("Youtube API ADAPTIVE"), id, QStringList() << query.toString(), true);

                        QStringList templates =
                            decodeStr(
                                query.queryItemValue(tkn_adaptive_fmts)
                            ).split(',');

                        proceedLinkTemplates(templates, options, "&");
                    }

                    if (options.isEmpty())
                        extractFromPage(id, options);

                    if (options.isEmpty())
                        return id;

                    FmtOption option = chooseFmtByQuality(options);
                    return option.toUrl();
                }
            };
        }
    }
}

#endif // YOUTUBE_API_MISC
