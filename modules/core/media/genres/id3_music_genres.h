#ifndef ID3_MUSIC_GENRES
#define ID3_MUSIC_GENRES

#include "genres_interface.h"
#include "modules/core/interfaces/singleton.h"

namespace Core {
    namespace Media {
        class Id3MusicGenres : public IGenres, public Singleton<Id3MusicGenres> {
        public:
            inline int defaultInt() const { return 12; }
        protected:
            void initDefault();
        private:
            friend class Singleton<Id3MusicGenres>;
            inline Id3MusicGenres() : IGenres() { initDefault(); }
        };
    }
}

#endif // ID3_MUSIC_GENRES
