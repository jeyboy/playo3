#ifndef SINGLE_APPLICATION_H
#define SINGLE_APPLICATION_H

#include <qapplication.h>
#include <qsharedmemory.h>
#include <qtimer.h>
#include <qbytearray.h>

class SingleApplication : public QApplication {
    Q_OBJECT
public:
    SingleApplication(int & argc, char * argv[], const QString uniqueKey);

    bool isRunning();
    bool sendMessage(const QString & message);

//    bool notify(QObject * receiver, QEvent * even);

public slots:
    void checkForMessage();

signals:
    void messageAvailable(QString message);

private:
    bool _isRunning;
    QSharedMemory sharedMemory;
};

#endif // SINGLE_APPLICATION_H
