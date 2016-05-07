#ifndef ISEARCHABLE
#define ISEARCHABLE

#include <qurl.h>
#include <qstringlist.h>
#include <qstringbuilder.h>
#include <qjsonobject.h>
#include <qjsonarray.h>

#include "modules/core/misc/format.h"
#include "modules/core/misc/logger.h"

#include "dialogs/user_action_dialog.h"
#include "settings.h"
#include "isource.h"
#include "igenreable.h"

#define MAX_PAGE 99
#define STYLES_MAX_PAGE 50

namespace Core {
    class ISearchable : public ISource, public IGenreable {
    public:
        enum PredicateType { in_title = 1, in_artist = 2, in_song = 4, in_tag = 8, in_owns = 16, in_originals = 32, in_foreign = 64, in_popular = 128, in_relative = 256 };

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
            inline bool by_relativity() const { return predicate_type & in_relative; }
        };

        inline QString lastError() const { return error; }

        inline ISearchable() { actionDialog = new UserActionDialog(Settings::obj().anchorWidget()); }
        inline virtual ~ISearchable() {}

        inline bool isSearchable() { return true; }

        enum ByTypeArg { sets, charts, soundtracks, by_genres, by_years, other, hits, fresh };

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

        virtual QJsonArray byChar(const QChar /*target_char*/, const SearchLimit & /*limitations*/) { return QJsonArray(); }

        virtual QJsonArray byType(const ByTypeArg /*target_type*/, const SearchLimit & /*limitations*/) { return QJsonArray(); }

        virtual QJsonArray popular(const QString & /*genre*/) { return QJsonArray(); }

        virtual QJsonArray related(const QString & /*predicate*/) { return QJsonArray(); }
    signals:
        void errorReceived(int, QString);
    protected:
        virtual QJsonArray search_postprocess(QString & /*predicate*/, QString & /*genre*/, const SearchLimit & /*limitations*/) = 0;

        QString error;
        UserActionDialog * actionDialog;
    };
}

#endif // ISEARCHABLE
