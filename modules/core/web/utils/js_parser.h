#ifndef JS_PARSER
#define JS_PARSER

#include <qstring.h>
#include "modules/pizduck/duktape.h"

namespace Core {
    namespace Web {
        namespace Js {
            class Document {
                duk_context * ctx;
            public:
                static QString proceedFuncScript(const QString & script) {
                    ctx = duk_create_heap_default();

                    duk_push_string(ctx, script.toLatin1().const_pointer());
                    duk_compile(ctx, DUK_COMPILE_FUNCTION);
//                    duk_push_int(ctx, 5); // push param1 for func
//                    duk_push_int(ctx, 5); // push param2 for func
                    duk_call(ctx, 0);      /* [ func ] -> [ result ] */
                    QString res(duk_get_string(ctx, -1));
//                    printf("program result: %lf\n", (double) duk_get_number(ctx, -1));
                    duk_pop(ctx);
                    duk_destroy_heap(ctx);
                    return res;
                }

//                static QString proceedEvalScript(const QString & script) {
//                    ctx = duk_create_heap_default();

//                    duk_push_string(ctx, script.toLatin1().const_pointer());
//                    duk_compile(ctx, DUK_COMPILE_EVAL);
//                    duk_call(ctx, 0);      /* [ func ] -> [ result ] */
//                    QString res(duk_get_string(ctx, -1));
////                    printf("program result: %lf\n", (double) duk_get_number(ctx, -1));
//                    duk_pop(ctx);
//                    duk_destroy_heap(ctx);
//                    return res;

//                    //                    duk_push_string(ctx, "2+3");
//                    //                    duk_push_string(ctx, "eval");
//                    //                    duk_compile(ctx, DUK_COMPILE_EVAL);
//                    //                    duk_call(ctx, 0);      /* [ func ] -> [ result ] */
//                    //                    printf("eval result: %lf\n", (double) duk_get_number(ctx, -1));
//                    //                    duk_pop(ctx);
//                }

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
