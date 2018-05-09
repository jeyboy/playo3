#ifndef LINKABLE
#define LINKABLE

#include <qstringbuilder.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

#define LINKBLE_UID_KEY QLatin1String("u")
#define LINKBLE_NAME_KEY QLatin1String("n")
#define LINKBLE_PERMALINK_KEY QLatin1String("p")
#define LINKBLE_IMAGE_KEY QLatin1String("i")

namespace Core {
    namespace Web {
        class Linkable {
            QString _uid;
            QString _human_name;
            QString _permalink;
            QString _image_url;
        public:
            Linkable(const QString & uid = QString(), const QString & human_name = QString(), const QString & permalink = QString(), const QString & image_url = QString())
                : _uid(uid), _human_name(human_name), _permalink(permalink), _image_url(image_url) {}

            QString uid() const { return _uid; }
            QString humanName() const { return _human_name; }
            QString permalink() const { return _permalink; }
            QString imageUrl() const { return _image_url; }

            QString permaTitle() const { return QStringLiteral(" aka %1").arg(_permalink.isEmpty() ? QLatin1String("?") : _permalink); }

            QJsonObject toJson() {
                QJsonObject root;

                root.insert(LINKBLE_UID_KEY, _uid);
                root.insert(LINKBLE_NAME_KEY, _human_name);
                root.insert(LINKBLE_PERMALINK_KEY, _permalink);
                if (!_image_url.isEmpty())
                    root.insert(LINKBLE_IMAGE_KEY, _image_url);

                return root;
            }

            static Linkable fromJson(const QJsonObject & json) {
                return Linkable(
                    json.value(LINKBLE_UID_KEY).toString(),
                    json.value(LINKBLE_NAME_KEY).toString(),
                    json.value(LINKBLE_PERMALINK_KEY).toString(),
                    json.value(LINKBLE_IMAGE_KEY).toString()
                );
            }
        };
    }
}

#endif // LINKABLE
