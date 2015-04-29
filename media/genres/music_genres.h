#ifndef MUSIC_GENRES
#define MUSIC_GENRES

#include "genres_interface.h"

class MusicGenres : public IGenres {
public:
    static MusicGenres * instance();
    inline static void close() { delete self; }
    inline int defaultInt() const { return 12; }
protected:
    void initDefault();
private:
    inline MusicGenres() : IGenres() {
        initDefault();
    }

    static MusicGenres * self;
};

#endif // MUSIC_GENRES
