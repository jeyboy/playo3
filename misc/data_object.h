#ifndef DATA_OBJECT
#define DATA_OBJECT

#include <qurl>
#include <qfile>

class DataObject : QObject {
    Q_OBJECT
public:
    DataObject(QUrl & url, uint buffer_length = 1024 * 1024);
    ~DataObject();

    QString lastError() const;

    virtual void open(QUrl url = QUrl()) = 0;
    virtual void download(QUrl savePath, int & progressIndicator) = 0;
//    bool openSync(QUrl url = QUrl());
    virtual qint64 lenght() = 0;
//    int read(void * buf, int & length);
//    qint64 seek(qint64 newPos = -1);
//    qint64 pos();

protected:
    void downloadProc(QUrl savePath);
    void initRequest(QUrl url);
    virtual void closeConnection();

signals:
    void start(void *);
    void end(void *, bool);
    void progress(void *, float percentage);

protected slots:
    void download(QUrl savePath);
    void proceedResponse();
    void onError(QString);

private:
    int bufferLength;
    QString error;
    QUrl obj_url;
};

#endif // DATA_OBJECT
