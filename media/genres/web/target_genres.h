#ifndef TARGET_GENRES
#define TARGET_GENRES

#include "../genres_interface.h"

class TargetGenres : public IGenres {
public:
    inline void addGenre(QString name, int code) { genres.insert(name, code); }
};

#endif // TARGET_GENRES
