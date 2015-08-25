#ifndef ID3_MUSIC_GENRES
#define ID3_MUSIC_GENRES

#include "genres_interface.h"

class Id3MusicGenres : public IGenres {
public:
    static Id3MusicGenres * instance();
    inline static void close() { delete self; }
    inline int defaultInt() const { return 12; }
protected:
    void initDefault();
private:
    inline Id3MusicGenres() : IGenres() { initDefault(); }

    static Id3MusicGenres * self;
};

#endif // ID3_MUSIC_GENRES
