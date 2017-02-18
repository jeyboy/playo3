//curl 'https://api.sendgrid.com/v3/stats?start_date=2017-02-17' --header 'Authorization: Bearer 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'   --header 'Content-Type: application/json'
#ifndef JS_PARSER
#define JS_PARSER

//http://duktape.org/api.html

#include <qstring.h>
#include "modules/pizduck/duktape.h"

namespace Core {
    namespace Web {
        namespace Js {
            class Document {
//                duk_context * ctx;
            public:
                static QString proceedJsCall(const QString & script, const QString & call, const QString & arg) {
                    duk_context * ctx = duk_create_heap_default();
                    QByteArray ar = (script + "\n" + call + "(\"" + arg + "\");").toLatin1();
                    duk_eval_string(ctx, ar.constData());
                    QString res(duk_get_string(ctx, -1));
                    duk_destroy_heap(ctx);
                    return res;
                }

//                Document(const QString & content) {
//                    ctx = duk_create_heap_default();
//                    duk_eval_string(ctx, content.toLatin1().const_pointer());
//                    printf("result is: %s\n", duk_get_string(ctx, -1));
//                    duk_pop(ctx);
//                }

//                ~Document() {
//                    duk_destroy_heap(ctx);
//                }
            };
        }
    }
}

#endif // JS_PARSER
