#ifndef FILENAME_CONVERSIONS_TEST
#define FILENAME_CONVERSIONS_TEST

#include <qtest.h>
#include "../modules/core/misc/file_utils/filename_conversions.h"

namespace Core {
    class TestFilenameConversions : public QObject {
        Q_OBJECT
    public:
        inline explicit TestFilenameConversions(QObject * parent = 0) : QObject(parent) {}

        void cacheTitleFilterTest_data();
        void forwardNumberFilterTest_data();
        void downloadTitleTest_data();

    private slots:
        void cacheTitleFilterTest();
        void forwardNumberFilterTest();
        void downloadTitleTest();
    };
}

#endif // FILENAME_CONVERSIONS_TEST
