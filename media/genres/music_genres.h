#ifndef MUSIC_GENRES
#define MUSIC_GENRES

#include "target_genres.h"
#include "misc/file_utils/data_store.h"

#define GENRES_PATH QStringLiteral("genres_list.json")
#define ROOT_VAL_NAME QStringLiteral("genres")
#define GENRE_VAL_NAME QStringLiteral("name")
#define GENRE_NUM_NAME QStringLiteral("num")

class MusicGenres : public TargetGenres { // global genres class // based on spotify genres list
public:
    inline ~MusicGenres() {
        save();
        delete settings;
    }
    static MusicGenres * instance();
    inline static void close() { delete self; }
    inline int defaultInt() const { return 12; }
protected:
    void initDefault();
private:
    Playo3::DataStore * settings;

    inline MusicGenres() : TargetGenres() { load(); }

    void load();
    void save();
    void prepare();

    static MusicGenres * self;
};

#endif // MUSIC_GENRES
