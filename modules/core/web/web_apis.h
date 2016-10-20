#ifndef WEB_APIS
#define WEB_APIS

#include "sources/_sources.h"

namespace Core {
    namespace Web {
        class Apis {
            static QMap<DataSubType, ISource *> sources;
            static QMap<DataSubType, QString> sources_name;
        public:
            static void initiate(const QJsonObject & obj);

            static QMap<DataSubType, ISource *> sourcesList() { return sources; }
            inline static ISource * source(const DataSubType & item_type) { return sources.value(DST_EXTRACT_FLAGS(item_type)); }
            inline static QString source_name(const DataSubType & item_type) { return sources_name.value(DST_EXTRACT_FLAGS(item_type)); }

            static void close(QJsonObject & obj);

            static QJsonValue run(const QString & cmd) {
                Cmd cmnd = Cmd(cmd);

                ISource * source = sources.value(DST_EXTRACT_FLAGS(cmnd.source_type));
                if (source == 0)
                    return QJsonObject();
                else
                    return source -> run((Core::ICmd::ICmdMethods)cmnd.mtd, cmnd.attrs);
            }

            static QString restoreUrl(const QString & refreshStr, const DataSubType & itemSubType, const DataMediaType & itemMediaType) {
                qDebug() << "RESTORING" << itemSubType << itemMediaType << refreshStr;

                ISource * source = sources.value(DST_EXTRACT_FLAGS(itemSubType));
                if (source == 0)
                    return QString();
                else
                    return source -> refresh(refreshStr, itemMediaType);
            }

//          need to ask user about choosing of his country and manually proceed it if request failed
//          enum QLocale::Country
//          QString QLocale::name() const
//          Returns the language and country of this locale as a string of the form "language_country", where language is a lowercase, two-letter ISO 639 language code, and country is an uppercase, two- or three-letter ISO 3166 country code.
            static QString userCountry() { // not tested
                QJsonObject obj = Manager::prepare() -> jsonGet(LSTR("http://ip-api.com/json"));
                if (JSON_HAS_KEY(obj, LSTR("countryCode")))
                    return JSON_STR(obj, LSTR("countryCode")).toLower();
                else return QString();
            }
        };
    }
}

#endif // WEB_APIS
