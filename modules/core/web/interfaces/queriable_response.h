#ifndef QUERIABLE_RESPONSE
#define QUERIABLE_RESPONSE

#include <qjsonarray.h>

namespace Core {
    namespace Web {
        struct QueriableResponse {
            QueriableResponse() {}

            QueriableResponse(const QJsonArray & content, int next_offset) : content(content), next_offset(next_offset)  {}

            QJsonArray content;
            int next_offset;
        };
    }
}

#endif // QUERIABLE_RESPONSE
