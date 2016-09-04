#ifndef QUERIABLE_RESPONSE
#define QUERIABLE_RESPONSE

#include <qjsonarray.h>

namespace Core {
    namespace Web {
        struct QueriableResponse {
            QueriableResponse() {}

            QueriableResponse(const QJsonArray & content, const QString & next_offset, int items_limit, int requests_limit, bool finished)
                : content(content), next_offset(next_offset), items_limit(items_limit),
                  requests_limit(requests_limit), finished(finished)  {}

            QJsonArray content;
            QString next_offset;
            int items_limit;
            int requests_limit;
            bool finished;
        };
    }
}

#endif // QUERIABLE_RESPONSE
