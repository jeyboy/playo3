#ifndef WEB_OBJECT_H
#define WEB_OBJECT_H

#include <qurl>
#include <qfile>
#include <QtConcurrent/QtConcurrent>
#include "custom_network_access_manager.h"

class WebObject : QObject {
    Q_OBJECT
public:
    WebObject(void * related_to, QUrl & url, uint buffer_length = 1024 * 1024);
    ~WebObject();

    QString lastError() const;

    void open(QUrl url = QUrl());
    bool openSync(QUrl url = QUrl());
    qint64 lenght();
    int read(void * buf, int & length);
    qint64 seek(qint64 newPos = -1);
    qint64 pos();

protected:
    void downloadProc(QUrl savePath);
    void initRequest(QUrl url);
    void closeConnection();

signals:
    void start(void *);
    void end(void *, bool);
    void progress(void *, float percentage);

protected slots:
    void download(QUrl savePath);
    void proceedResponse();
    void onSslErrors(const QList<QSslError> &errors);
    void onError(QNetworkReply::NetworkError);
    void onError(QString);

private:
    void * relation;
    int bufferLength;
    QString error;
    QNetworkReply * m_http;
    QUrl obj_url;
    CustomNetworkAccessManager webManager;
};

#endif // WEB_OBJECT_H
