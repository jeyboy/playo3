#ifndef MYZUKA_ALBUM
#define MYZUKA_ALBUM

#include "igrabber_api.h"

class MyzukaAlbum : public  IGrabberApi {
protected:
    QString baseUrlStr(QString & predicate = DEFAULT_PREDICATE_NAME) { return "https://myzuka.org" + predicate; }
    void toJson(QNetworkReply * reply, QJsonObject & json) {
//        QByteArray ar = reply -> readAll();


    }
private:
    inline QString trackPredicate() { return "div"; }
};

#endif // MYZUKA_ALBUM
