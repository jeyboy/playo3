#ifndef FOURSHARED_ITEM
#define FOURSHARED_ITEM

#include "fourshared_defines.h"

namespace Core {
    namespace Web {
        namespace Fourshared {
            class Item : public Base {
            public:
                QJsonValue itemsByCollection(const QString & collection_id, const std::initializer_list<std::pair<QString, QString> > & block_params = {}) {
                    QJsonArray block_content = saRequest(
                        baseUrlStr(
                            qst_site_base, LSTR("web/accountActions/changeDir"),
                            {{ LSTR("dirId"), collection_id }}
                        ),
                        call_type_json, 0, proc_json_proc,
                        QStringList(), call_method_post, siteHeaders()
                    );

                    return prepareBlock(dmt_any, block_content, block_params);
                }

                QJsonValue itemInfo(const DataMediaType & media_type, const QString & id) {
                    QString url = baseUrlStr(
                        qst_site_base, LSTR("web/link/download/file"), {{LSTR("fileId"), id}}
                    );

                    QJsonObject res = Manager::prepare() -> jsonPost(url, siteHeaders());
                    url = JSON_STR(res, LSTR("link"));

                    Response * response = Manager::prepare() -> getFollowed(url, siteHeaders());
                    QString err;
                    QJsonArray arr;
                    QueriableArg arg(&arr, url, call_type_html, media_type == dmt_audio ? proc_tracks2 : proc_video2);
                    htmlToJson(&arg, response, err);

                    return arr;
                }
            };
        }
    }
}

#endif // FOURSHARED_ITEM
