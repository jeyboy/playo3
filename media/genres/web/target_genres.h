#ifndef TARGET_GENRES
#define TARGET_GENRES

#include "../genres_interface.h"

class TargetGenres : public IGenres {
public:
    inline void addGenre(QString name, int code) { genres.insert(name, code); }
    inline void addGenre(QString name, int code, QString alias) { addGenre(name, code); aliases.insert(code, alias); }
    inline void addGenre(QString name, QString alias) {
        int code = genres.size();
        addGenre(name, code); aliases.insert(code, alias);
    }
    inline bool isEmpty() { return genres.isEmpty(); }
    inline QString toString(int id) const { return genres.key(id, QStringLiteral("")); }
    inline QString toAlias(int id) const { return aliases.value(id, QStringLiteral("")); }
private:
    QHash<int, QString> aliases;
};

#endif // TARGET_GENRES
