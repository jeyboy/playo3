#ifndef HTML_PARSET_TEST
#define HTML_PARSET_TEST

#include <qtest.h>
#include <qfile.h>

#include "../modules/core/web/utils/html_parser.h"

namespace Core {
    namespace Web {
        namespace Html {
            class TestHtmlParser : public QObject {
                Q_OBJECT
            public:
                inline explicit TestHtmlParser(QObject * parent = 0) : QObject(parent) {}

            private slots:
                void selectorParseTest_data();
                void selectorParseTest();

                void pageParseTest_data();
                void pageParseTest();
            };
        }
    }
}

#endif // HTML_PARSET_TEST
