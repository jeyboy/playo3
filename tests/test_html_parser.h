#ifndef HTML_PARSET_TEST
#define HTML_PARSET_TEST

#include <qtest.h>
#include "../modules/core/web/utils/html_parser.h"

namespace Core {
    namespace Web {
        namespace Html {
            class TestHtmlParser : public QObject {
                Q_OBJECT
            public:
                inline explicit TestHtmlParser(QObject * parent = 0) : QObject(parent) {}

                void selectorParseTest_data();

            private slots:
                void selectorParseTest();
            };
        }
    }
}

#endif // HTML_PARSET_TEST
