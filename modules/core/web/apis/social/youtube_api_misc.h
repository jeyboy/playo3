#ifndef YOUTUBE_API_MISC
#define YOUTUBE_API_MISC

#include "modules/core/web/interfaces/iapi.h"
#include "modules/core/web/utils/js_parser.h"
#include "youtube_api_keys.h"

namespace Core {
    namespace Web {
        namespace Youtube {
            class Misc : public IApi {
                enum Formats {
                    other = 0,
                    video_flv_400_240 = 5,
                    video_flv_450_270 = 6,
                    video_3gp_x_x = 13,
                    video_3gp_176_144 = 17,
                    video_mp4_640_360 = 18,
                    video_mp4_1280_720 = 22,
                    video_flv_640_360 = 34,
                    video_flv_854_480 = 35,
                    video_3gp_320_240 = 36,
                    video_mp4_1920_1080 = 37,
                    video_mp4_4096_3072 = 38,
                    video_webm_640_360 = 43,
                    video_webm_854_480 = 44,
                    video_webm_1280_720 = 45,
                    video_webm_1920_1080 = 46,

                    video_mp4_3d_x_360 = 82,
                    video_mp4_3d_x_480 = 83,
                    video_mp4_3d_x_720 = 84,
                    video_mp4_3d_x_1080 = 85,
                    video_webm_3d_x_360 = 100,
                    video_webm_3d_x_480 = 101,
                    video_webm_3d_x_720 = 102,

                    video_mp4_live_x_240_1 = 92,
                    video_mp4_live_x_360 = 93,
                    video_mp4_live_x_480 = 94,
                    video_mp4_live_x_720 = 95,
                    video_mp4_live_x_1080 = 96,
                    video_mp4_live_x_240_2 = 132,
                    video_mp4_live_x_72 = 151,

                    video_mp4_dash_x_240 = 133,
                    video_mp4_dash_x_360 = 134,
                    video_mp4_dash_x_480 = 135,
                    video_mp4_dash_x_720_1 = 136,
                    video_mp4_dash_x_1080_1 = 137,
                    video_mp4_dash_x_vary = 138,
                    video_mp4_dash_x_144 = 160,
                    video_mp4_dash_x_1440 = 264,
                    video_mp4_dash_x_720_2 = 298, // 60 fps
                    video_mp4_dash_x_1080_2 = 299, // 60 fps
                    video_mp4_dash_x_2160 = 266,

                    audio_m4a_dash_44_48 = 139, // aac
                    audio_m4a_dash_44_128 = 140, // aac
                    audio_m4a_dash_44_256 = 141, // aac

                    video_webm_dash_640_360 = 167,
                    video_webm_dash_854_480_1 = 168,
                    video_webm_dash_1280_720 = 169,
                    video_webm_dash_1920_1080 = 170,
                    video_webm_dash_854_480_2 = 218,
                    video_webm_dash_854_480_3 = 219,
                    video_webm_dash_x_144 = 278,
                    video_webm_dash_x_240 = 242,
                    video_webm_dash_x_360 = 243,
                    video_webm_dash_x_480_1 = 244,
                    video_webm_dash_x_480_2 = 245,
                    video_webm_dash_x_480_3 = 246,
                    video_webm_dash_x_720_1 = 247,
                    video_webm_dash_x_1080_1 = 248,
                    video_webm_dash_x_1440_1 = 271,
                    video_webm_dash_x_2160_1 = 272,
                    video_webm_dash_x_720_2 = 302, // 60 fps
                    video_webm_dash_x_1080_2 = 303, // 60 fps
                    video_webm_dash_x_1440_2 = 308, // 60 fps
                    video_webm_dash_x_2160_2 = 313,
                    video_webm_dash_x_2160_3 = 315, // 60 fps

                    audio_webm_dash_44_128 = 171, // aac
                    audio_webm_dash_44_256 = 172, // aac

                    audio_webm_dash_x_50 = 249, // opus
                    audio_webm_dash_x_70 = 250, // opus
                    audio_webm_dash_x_160 = 251, // opus

//                    video_rtmp = "_rtmp" // not ebable
                };

                struct JsMethod {
                    QString name;
                    QString code;
                };

                struct FmtOption {
                    FmtOption() : quality_id(0), audio(false), adaptive(false), decode_required(false) {}

                    QString toUrl() {
                        //                    !url.contains("ratebypass"))
                        //                            {
                        //                                url = url.append("&ratebypass=yes");
                        //                            }

                        if (decode_required)
                            return url % QStringLiteral("&signature=") % signature;

                        return url;
                    }

                    bool isEmpty() { return url.isEmpty(); }

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

                void extractFromPage(const QString & id, QHash<int, FmtOption> & options) {
                    //there should be 2 variants:
                        //  str has 's' param, which should be decoded first with usage of separate js file
                            // http://stackoverflow.com/questions/21510857/best-approach-to-decode-youtube-cipher-signature-using-php-or-js
                            // http://www.codeproject.com/Articles/2352/JavaScript-call-from-C
                            // https://github.com/bitnol/CipherAPI

                    QString response = Web::Manager::prepare() -> followedGet(url_embed.arg(id)) -> toText();

                    QString jsUrl;
                    if (extractJsUrl(response, jsUrl)) {
                        JsMethod js_method = extractJsMethod(jsUrl);

                        if (!js_method.code.isEmpty()) {
                            int pos = response.indexOf(tkn_url_encoded_fmt_stream_map);
                            if (pos != -1) {
                                pos += tkn_url_encoded_fmt_stream_map.length() + 3;
                                int len = response.indexOf('"', pos) - pos;
                                QStringList templates = response.mid(pos, len).split(',');
                                proceedLinkTemplates(templates, options, QStringLiteral("\\u0026"), js_method);
                            }

                            pos = response.indexOf(tkn_adaptive_fmts);
                            if (pos != -1) {
                                pos += tkn_adaptive_fmts.length() + 3;
                                int len = response.indexOf('"', pos) - pos;
                                QStringList templates = response.mid(pos, len).split(',');
                                proceedLinkTemplates(templates, options, QStringLiteral("\\u0026"), js_method);
                            }
                        }
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

                void extractJSMethod(const QString & name, const QString & js, QList<JsMethod> & jsMethods, QHash<QString, bool> locals = QHash<QString, bool>()) {
                    if (locals.contains(name)) return;

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
                        if (!expression.cap(2).isEmpty())
                            descriptor = descriptor.right(descriptor.length() - expression.cap(2).length());

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
                            /*expressions.append(parameters.split(','));*/
                            foreach(QString arg, parameters.split(','))
                                locals.insert(arg, true);

                            while (pos != -1) {
                                if (expressions.indexOf(expression.cap(1)) == -1)
                                    extractJSMethod(expression.cap(1), js, jsMethods, locals);
                                pos = expression.indexIn(method.code, pos + expression.cap(0).length());
                            }
                        }
                    }
                }


                JsMethod extractJsMethod(const QString & jsUrl) {
                    QString js = Web::Manager::prepare() -> followedGet(jsUrl) -> toText();

                    QString methodName;
                    JsMethod res;

                    QRegExp expression("signature=|set\\(\"signature\",(.+)\\("); // \\.sig\\|\\|([a-zA-Z0-9$]+)\\(
                    expression.setMinimal(true);
                    if (expression.indexIn(js) != -1) {
                        methodName = expression.cap(1);
                        QList<JsMethod> jsMethods;
                        extractJSMethod(methodName, js, jsMethods);

                        QString resJs;
                        for (QList<JsMethod>::Iterator meth = jsMethods.begin(); meth != jsMethods.end(); meth++)
                            resJs.append((*meth).code).append(";");

                        res.name = methodName;
                        res.code = resJs/*.replace(QStringLiteral("\\\""), QStringLiteral("\""))*/;
                    }

                    return res;
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

                FmtOption chooseFmtByQuality(QHash<int, FmtOption> options) {
                    // bass library did not support any audio quality
                    QList<int> qualities;
                    qualities // qualities, which supported by bass
                            << 13 << 17 << 18
                            << 22 << 36 << 37 << 38
                            << 82 << 83 << 84
                            << 85 << 133 << 134
                            << 135 << 136 << 137
                            << 138 << 160 << 264
                            << 298 << 299 << 266;

                    for(QList<int>::Iterator quality = qualities.begin(); quality != qualities.end(); quality++)
                        if (options.contains(*quality))
                            return options[*quality];

                    qDebug() << "NO QUALITY";
                    return FmtOption();
                }

                void proceedLinkTemplates(const QStringList & templates, QHash<int, FmtOption> & options, const QString & splitter, const JsMethod & js = JsMethod()) {
                    for(QStringList::ConstIterator item = templates.cbegin(); item != templates.cend(); item++) {
                        qDebug() << (*item);
                        FmtOption opt;

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
                                opt.signature = Js::Document::proceedJsCall(js.code, js.name, opt.signature);
                                opt.decode_required = true;
                            }
                            else if ((*part).startsWith("itag="))
                                opt.quality_id = (*part).mid(5).toInt();
                            else if ((*part).startsWith("type=")) // each link contains param type equal to the 'video' or 'audio'
                                opt.audio = (*part).mid(5, 5) == QStringLiteral("audio");
                        }

                        options.insert(opt.quality_id, opt);
                    }


//                    .replace("\\u0026", "&")
                }
            protected:
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
                    return option.isEmpty() ? id : option.toUrl();
                }
            };
        }
    }
}

#endif // YOUTUBE_API_MISC
