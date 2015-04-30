#ifndef FILENAME_CONVERSIONS
#define FILENAME_CONVERSIONS

#include <qobject.h>

class TestFilenameConversions : public QObject {
    Q_OBJECT
public:
    inline explicit TestFilenameConversions(QObject * parent = 0) : QObject(parent) {}

private slots:
    void cacheTitleFilter();
    void forwardNumberFilter();
    void downloadTitle();
};

#endif // FILENAME_CONVERSIONS
