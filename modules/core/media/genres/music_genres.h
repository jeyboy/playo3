#ifndef MUSIC_GENRES
#define MUSIC_GENRES

#include "target_genres.h"
#include "modules/core/misc/file_utils/data_store.h"
#include "modules/core/interfaces/singleton.h"

#define GENRES_PATH QLatin1String("genres_list.json")
#define ROOT_VAL_NAME QLatin1String("genres")
#define GENRE_VAL_NAME QLatin1String("name")
#define GENRE_NUM_NAME QLatin1String("num")

namespace Core {
    namespace Media {
        class MusicGenres : public TargetGenres, public Singleton<MusicGenres> { // global genres class // based on spotify genres list
        public:
            inline ~MusicGenres() {
                save();
                delete settings;
            }
            inline int defaultInt() const { return 12; }
        protected:
            void initDefault();
        private:
            DataStore * settings;

            friend class Singleton<MusicGenres>;
            inline MusicGenres() : TargetGenres() { load(); }

            void load();
            void save();
            void prepare();
        };
    }
}

#endif // MUSIC_GENRES
