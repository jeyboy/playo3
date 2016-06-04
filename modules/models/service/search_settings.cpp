#include "search_settings.h"

//#include <qjsonobject.h>

////#include "modules/models/model_interface.h"
//#include "dockbars.h"
//#include "modules/core/web/web_apis.h"

//using namespace Core;

//QString SearchRequest::token() {
//    bool has_predicate = !spredicate.isEmpty();
//    bool has_genre = !sgenre.isEmpty();

//    if (has_predicate) {
//        if (has_genre)
//            return spredicate % QStringLiteral(" (") % sgenre % QStringLiteral(") ");
//        else
//            return spredicate;
//    } else if (has_genre) return sgenre;
//    else if (popular && search_type == remote) return QStringLiteral("Popular");
//    else return QStringLiteral("All");
//}

//void SearchRequest::fromJson(const QJsonArray & objs, QList<SearchRequest> & list) {
//    QHash<QString, QAbstractItemModel *> inners;

//    QList<Controls::DockBar *> bars = Presentation::Dockbars::obj().dockbars();

//    for(QList<Controls::DockBar *>::Iterator bar = bars.begin(); bar != bars.end(); bar++) {
//        Views::IView * v = Presentation::Dockbars::obj().view(*bar);
//        if (v) inners.insert((*bar) -> objectName(), v -> model());
//    }

//    for(QJsonArray::ConstIterator request = objs.constBegin(); request != objs.constEnd(); request++) {
//        QJsonObject obj = (*request).toObject();
//        RequestType search_type = (RequestType)obj.value(JSON_SEARCH_TYPE).toInt();
//        void * subject;

//        switch(search_type) {
//            case inner: {
//                subject = inners.value(obj.value(JSON_SEARCH_SUBJECT).toString());
//            break;}

//            case remote: {
//                int subject_id = obj.value(JSON_SEARCH_SUBJECT).toInt();
//                subject = Web::Apis::searcher((DataSubType)subject_id);
//            break;}

//            default: {
//                QString subject_str = obj.value(JSON_SEARCH_SUBJECT).toString();
//                subject = new QString(subject_str);
//            break;}
//        }

//        list << SearchRequest(
//            search_type,
//            subject,
//            obj.value(JSON_SEARCH_TYPE).toInt(),
//            obj.value(JSON_SEARCH_CONTENT_LIMIT).toInt(),
//            obj.value(JSON_SEARCH_PREDICATE).toString(),
//            obj.value(JSON_SEARCH_GENRE).toString()
//        );
//    }
//}

//void SearchRequest::save(QJsonArray & arr) {
//    QJsonObject self;

//    switch(search_type) {
//        case inner: {
//            QString subject = ((Models::IModel *)(search_interface)) -> objectName();
//            subject = subject.mid(0, subject.length() - MODEL_POSTFIX.length());
//            self.insert(JSON_SEARCH_SUBJECT, subject);
//        break;}

//        case remote: {
//            int subject = ((ISearchable *)(search_interface)) -> siteType();
//            self.insert(JSON_SEARCH_SUBJECT, subject);
//        break;}

//        default: {
//            QString subject = *(QString *)(search_interface);
//            self.insert(JSON_SEARCH_SUBJECT, subject);
//        break;}
//    }

//    self.insert(JSON_SEARCH_TYPE, search_type);
//    self.insert(JSON_SEARCH_CONTENT_LIMIT, content_type);

//    if (!spredicate.isEmpty())
//        self.insert(JSON_SEARCH_PREDICATE, spredicate);

//    if (!sgenre.isEmpty())
//        self.insert(JSON_SEARCH_GENRE, sgenre);

//    if (popular)
//        self.insert(JSON_SEARCH_TYPE, source_type);

//    arr << self;
//}
