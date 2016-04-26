#ifndef TARGET_GENRES
#define TARGET_GENRES

#include "genres_interface.h"

namespace Core {
    namespace Media {
        class TargetGenres : public IGenres {
        public:
            inline void addGenre(const QString & name) { addGenre(name, genres.size()); }
            inline void addGenre(const QString & name, int code) { genres.insert(name, code); }
            inline void addGenre(const QString & name, int code, QString alias) { addGenre(name, code); aliases.insert(code, alias); }
            inline void addGenre(const QString & name, QString alias) {
                int code = genres.size();
                addGenre(name, code); aliases.insert(code, alias);
            }
            inline bool isEmpty() { return genres.isEmpty(); }
            inline QString toString(const int & id) const { return genres.key(id, QString()); }
            inline QString toAlias(const int & id) const { return aliases.value(id, QString()); }
            inline QString toAlias(const QString & name) const { return toAlias(genres.value(name, -1)); }
        private:
            QHash<int, QString> aliases;
        };
    }
}

#endif // TARGET_GENRES
