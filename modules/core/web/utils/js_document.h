#ifndef JS_PARSER
#define JS_PARSER

//http://duktape.org/api.html

#include <qstring.h>
#include "modules/pizduck/duktape.h"

namespace Core {
    namespace Web {
        namespace Js {
            class Document {
            public:
                static QString proceedJsCall(const QString & script, const QString & call, const QString & arg) {
                    duk_context * ctx = duk_create_heap_default();

                    qDebug() << arg;

                    QByteArray ar = (script + "\n" + call + "(\"" + arg + "\");").toLatin1(); // toUtf8()
//                    char * script_data = new char[ar.length() + 1]; // + 1 for zero in the end of string
//                    memcpy(script_data, ar.constData(), ar.length());
//                    script_data[ar.length()] = '\0';

                    duk_eval_string(ctx, ar.constData());
                    QString res(duk_get_string(ctx, -1));
                    duk_destroy_heap(ctx);
                    return res;
                }
            };
        }
    }
}

#endif // JS_PARSER
