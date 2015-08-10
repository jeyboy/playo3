#ifndef TARGET_GENRES
#define TARGET_GENRES

#include "../genres_interface.h"

class TargetGenres : public IGenres {
public:
    inline void addGenre(QString name, int code) { genres.insert(name, code); }
    inline bool isEmpty() { return genres.isEmpty(); }
    inline QString toString(int id) const { return genres.key(id, QStringLiteral("")); }
};

#endif // TARGET_GENRES
