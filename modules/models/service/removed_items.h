#ifndef REMOVED_ITEMS
#define REMOVED_ITEMS

#include <qhash>
#include <qdatetime.h>
#include <qtimer>

#define DROP_MS_INTERVAL 600000 // 10 min

class RemovedItems : public QObject {
    Q_OBJECT

    QHash<quint32, quint64> addresses;
    QTimer timer;
public:
    RemovedItems() {
        connect(&timer, SIGNAL(timeout()), this, SLOT(clearOldItems()));
        timer.start(DROP_MS_INTERVAL);
    }
    ~RemovedItems() { timer.stop(); }

    void registerItem(void * item) {
        addresses.insert((quint32)item, QDateTime::currentMSecsSinceEpoch());
    }

    bool containsItem(void * item) {
        return addresses.contains((quint32)item);
    }
protected slots:
    void clearOldItems() {
        QMutableHashIterator<quint32, quint64> i(addresses);
        quint64 timestamp = QDateTime::currentMSecsSinceEpoch();

        while(i.hasNext()) {
            i.next();

            if (timestamp - i.value() >= DROP_MS_INTERVAL)
                i.remove();
        }
    }
};

#endif // REMOVED_ITEMS
