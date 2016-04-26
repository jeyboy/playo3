#ifndef ISEARCHABLE
#define ISEARCHABLE

#include <qurl.h>
#include <qstringlist.h>
#include <qstringbuilder.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

#include "modules/core/misc/format.h"
#include "modules/core/misc/logger.h"
#include "modules/core/web/utils/web_manager.h"
#include "modules/core/data_sub_types.h"

#include "dialogs/user_action_dialog.h"
#include "settings.h"
#include "isource.h"
#include "igenreable.h"

#define DEFAULT_PREDICATE_NAME QString()
#define REQUEST_DELAY 260 // ms
#define MAX_PAGE 99
#define STYLES_MAX_PAGE 50

namespace Core {
    class ISearchable : public ISource, public IGenreable {
    public:
        enum PredicateType { in_title = 1, in_artist = 2, in_song = 4, in_tag = 8, in_owns = 16, in_originals = 32, in_foreign = 64, in_popular = 128 };

        struct SearchLimit {
            SearchLimit(PredicateType predicate_type, int total_limit, int start_page = 1, int page_amount = MAX_PAGE) :
                total_limit(total_limit), start_page(start_page), count_page(page_amount), predicate_type(predicate_type) {}

            int total_limit;
            int start_page;
            int count_page;
            PredicateType predicate_type;

            inline bool by_artists() const { return predicate_type & in_artist; }
            inline bool by_titles() const { return predicate_type & in_title; }
            inline bool by_songs() const { return predicate_type & in_song; }
            inline bool by_tags() const { return predicate_type & in_tag; }

            inline bool by_popularity() const { return predicate_type & in_popular; }
            inline bool by_owns() const { return predicate_type & in_owns; }
            inline bool by_originals() const { return predicate_type & in_originals; }
            inline bool by_foreign() const { return predicate_type & in_foreign; }
        };

        inline QString lastError() const { return error; }

        inline ISearchable() { actionDialog = new UserActionDialog(Settings::obj().anchorWidget()); }

        inline virtual ~ISearchable() {}

        inline QString encodeStr(const QString & str) const { return QUrl::toPercentEncoding(str); }
        inline QString decodeStr(const QString & str) const { return QUrl::fromPercentEncoding(str.toLatin1()); }

        enum ByTypeArg { sets, charts, soundtracks, by_genres, by_years, other, hits, fresh };

        virtual void toJson(QJsonObject & /*hash*/) { qDebug() << name() << "TO JSON"; } // stub
        virtual void fromJson(const QJsonObject & /*hash*/) { qDebug() << name() << "FROM JSON"; } // stub

        virtual QJsonArray search(QString & predicate, QString & genre, const SearchLimit & limitations) {
            if (!predicate.isEmpty()) {
                return search_postprocess(predicate, genre, limitations);
            } else if (!genre.isEmpty())
                return byGenre(genre, limitations);
            else if (limitations.by_popularity() || predicate.isEmpty())
                return popular(genre);

            return QJsonArray();
        }

        virtual QJsonArray byGenre(const QString & /*genre*/, const SearchLimit & /*limitations*/) { return QJsonArray(); }

        virtual QJsonArray byChar(QChar /*target_char*/, const SearchLimit & /*limitations*/) { return QJsonArray(); }

        virtual QJsonArray byType(ByTypeArg /*target_type*/, const SearchLimit & /*limitations*/) { return QJsonArray(); }

        virtual QJsonArray popular(QString & /*genre*/) { return QJsonArray(); }

        //    virtual QJsonArray related(QUrl /*target_page*/) { return QJsonArray(); }

        virtual QString refresh(const QString & refresh_page) = 0;
    signals:
        void errorReceived(int, QString);
    protected:
        void concatJsonArrays(QJsonArray & ret, const QJsonArray & items) {
            for(QJsonArray::ConstIterator it = items.constBegin(); it != items.constEnd(); it++)
                ret.append(*it);
        }

        virtual QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) = 0;
        QUrl baseUrl(const QString & predicate, const QUrlQuery & query) {
            QUrl url(baseUrlStr(predicate));
            url.setQuery(query);
            return url;
        }
        virtual inline QUrlQuery genDefaultParams() { return QUrlQuery(); }

        virtual QJsonArray search_postprocess(QString & /*predicate*/, QString & /*genre*/, const SearchLimit & /*limitations*/) = 0;

        QString error;
        UserActionDialog * actionDialog;
    };
}

#endif // ISEARCHABLE
