#ifndef MUSIC_GENRES
#define MUSIC_GENRES

#include "genres_interface.h"

class MusicGenres : public IGenres {
    static MusicGenres * instance();
    static void close() {
        delete self;
    }
private:
    inline int defaultInt() const { return 12; }
    void initDefault();

    inline MusicGenres() : IGenres() {}

    MusicGenres * self;
};

#endif // MUSIC_GENRES
