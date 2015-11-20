#ifndef ISEARCHABLE
#define ISEARCHABLE

#include <qdebug.h>
#include <qurl.h>
#include <qstringlist.h>
#include <qstringbuilder.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

#include "modules/core/misc/format.h"
#include "modules/core/misc/logger.h"
#include "modules/core/web/utils/web_manager.h"
#include "modules/core/media/genres/target_genres.h"
#include "modules/core/web/web_sub_types.h"

#include "dialogs/user_action_dialog.h"
#include "settings.h"

#define DEFAULT_PREDICATE_NAME QString()
#define REQUEST_DELAY 260 // ms
#define MAX_PAGE 999
#define STYLES_MAX_PAGE 50

namespace Core {
    using namespace Media;

    class ISearchable {
    public:
        enum PredicateType { in_title = 1, in_artist = 2, in_song = 4, in_tag = 8, in_owns = 16, in_originals = 32, in_foreign = 64, in_popular = 128 };

        struct SearchLimit {
            SearchLimit(PredicateType predicate_type, int limit, int start_page = 1, int page_amount = MAX_PAGE) :
                count(limit), spage(start_page), cpage(page_amount), predicate_type(predicate_type) {}

            int count;
            int spage;
            int cpage;
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

        inline virtual bool isConnected() { return true; }

        inline QString encodeStr(const QString & str) const { return QUrl::toPercentEncoding(str); }

        enum ByTypeArg { sets, charts, soundtracks, by_genres, by_years, other, hits, fresh };

        virtual QString name() const = 0;

        virtual Web::SubType siteType() = 0;

        virtual void toJson(QJsonObject & /*hash*/) { qDebug() << name() << "TO JSON"; } // stub
        virtual void fromJson(const QJsonObject & /*hash*/) { qDebug() << name() << "FROM JSON"; } // stub

        virtual QJsonArray search(QString & predicate, QString & genre, const SearchLimit & limitations) {
            if (!predicate.isEmpty()) {
                return search_postprocess(predicate, genre, limitations);
            } else if (!genre.isEmpty())
                return byGenre(genre, limitations);
            else if (limitations.by_popularity())
                return popular();

            return QJsonArray();
        }

        inline TargetGenres genresList() {
            if (genres.isEmpty()) genres_prepocessing();
            return genres;
        }

        virtual inline QHash<QString, QString> friendsList() const { return QHash<QString, QString>(); } // stub
        virtual inline QHash<QString, QString> groupsList() const { return QHash<QString, QString>(); } // stub

        virtual QJsonArray byGenre(const QString & /*genre*/, const SearchLimit & /*limitations*/) { return QJsonArray(); }

        virtual QJsonArray byChar(QChar /*target_char*/, const SearchLimit & /*limitations*/) { return QJsonArray(); }

        virtual QJsonArray byType(ByTypeArg /*target_type*/, const SearchLimit & /*limitations*/) { return QJsonArray(); }

        virtual QJsonArray popular() { return QJsonArray(); }

        //    virtual QJsonArray related(QUrl /*target_page*/) { return QJsonArray(); }

        virtual QString refresh(QString refresh_page) = 0;
    signals:
        void errorReceived(int, QString);
    protected:
        void concatJsonArrays(QJsonArray & ret, const QJsonArray & items) {
            for(QJsonArray::ConstIterator it = items.constBegin(); it != items.constEnd(); it++)
                ret.append(*it);
        }

        virtual QString baseUrlStr(const QString & predicate = DEFAULT_PREDICATE_NAME) = 0;
        QUrl baseUrl(QString predicate, QUrlQuery & query) {
            QUrl url(baseUrlStr(predicate));
            url.setQuery(query);
            return url;
        }
        virtual inline QUrlQuery genDefaultParams() { return QUrlQuery(); }

        virtual QJsonArray search_postprocess(QString & /*predicate*/, QString & /*genre*/, const SearchLimit & /*limitations*/) = 0;
        virtual void genres_prepocessing() {}

        TargetGenres genres;

        QString error;
        UserActionDialog * actionDialog;
    };
}

#endif // ISEARCHABLE
