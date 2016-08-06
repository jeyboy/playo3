#ifndef WEB_APIS
#define WEB_APIS

#include "sources/_sources.h"

namespace Core {
    namespace Web {
        class Apis {
            static QMap<DataSubType, ISource *> sources;
        public:
            static void initiate(const QJsonObject & obj);

            static QMap<DataSubType, ISource *> sourcesList() { return sources; }
            inline static ISource * source(const DataSubType & item_type) { return sources.value(item_type); }

            static void close(QJsonObject & obj);

            static QJsonValue run(const QString & cmd) {
                QUrl url = ICmd::cmdToParams(cmd);

                ISource * source = sources.value((DataSubType)url.scheme().toInt());
                if (source == 0)
                    return QJsonObject();
                else
                    return source -> run((Core::ICmd::ICmdMethods)url.path().toInt(), url.query());
            }

            static void extractSourceTypeAndMediaType(const QString & cmd, int & source_type, int & media_type) {
                QUrl url = ICmd::cmdToParams(cmd);

                source_type = url.scheme().toInt();
                media_type = url.host().toInt();
            }

            static QString restoreUrl(const QString & refreshStr, const DataSubType & itemSubType, const DataMediaType & itemMediaType) {
                qDebug() << "RESTORING" << itemSubType << itemMediaType << refreshStr;

                ISource * source = sources.value(itemSubType);
                if (source == 0)
                    return QString();
                else
                    return source -> refresh(refreshStr, itemMediaType);
            }
        };
    }
}

#endif // WEB_APIS
