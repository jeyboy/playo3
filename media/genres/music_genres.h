#ifndef MUSIC_GENRES
#define MUSIC_GENRES

#include "genres_interface.h"

class MusicGenres : public IGenres {
public:
    static MusicGenres * instance();
    static void close() {
        delete self;
    }
protected:
    inline int defaultInt() const { return 12; }
    void initDefault();
private:
    inline MusicGenres() : IGenres() {
        initDefault();
    }

    static MusicGenres * self;
};

#endif // MUSIC_GENRES
