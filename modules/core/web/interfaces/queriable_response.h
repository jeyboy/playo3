#ifndef QUERIABLE_RESPONSE
#define QUERIABLE_RESPONSE

#include <qjsonarray.h>

namespace Core {
    namespace Web {
        struct QueriableResponse {
            QueriableResponse() {}

            QueriableResponse(const QJsonArray & content, int next_offset, int items_limit, int requests_limit)
                : content(content), next_offset(next_offset), items_limit(items_limit), requests_limit(requests_limit)  {}

            QJsonArray content;
            int next_offset;
            int items_limit;
            int requests_limit;

            bool isFinished() const { return content.size() < items_limit; }
        };
    }
}

#endif // QUERIABLE_RESPONSE
