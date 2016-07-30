#ifndef FILENAME_CONVERSIONS_TEST
#define FILENAME_CONVERSIONS_TEST

#include <qtest.h>
#include "../modules/core/misc/file_utils/filename_conversions.h"
#include "../modules/core/misc/file_utils/extensions.h"

namespace Core {
    class TestFilenameConversions : public QObject {
        Q_OBJECT
    public:
        inline explicit TestFilenameConversions(QObject * parent = 0) : QObject(parent) {}
    private slots:
        void extractExtension();
        void extractExtension_data();

        void cacheTitleFilterTest();
        void cacheTitleFilterTest_data();

        void forwardNumberFilterTest();
        void forwardNumberFilterTest_data();

        void downloadTitleTest();
        void downloadTitleTest_data();
    };
}

#endif // FILENAME_CONVERSIONS_TEST
