#ifndef IGENREABLE
#define IGENREABLE

#include "modules/core/media/genres/target_genres.h"

namespace Core {
    using namespace Media;

    class IGenreable {
    public:
        inline TargetGenres genresList() {
            if (genres.isEmpty()) genres_prepocessing();
            return genres;
        }
    protected:
        virtual void genres_prepocessing() {}

        TargetGenres genres;
    };
}

#endif // IGENREABLE
