#ifndef VK_GENRES
#define VK_GENRES

#include "target_genres.h"
#include "modules/core/interfaces/singleton.h"

namespace Core {
    namespace Media {
        class VkGenres : public TargetGenres, public Singleton<VkGenres> {
        public:
            int fromStandartId(int genreId);
            int toStandartId(int vkGenreId);
            QList<int> toStandartIds(int vkGenreId);

            inline int defaultInt() const { return 18; }
        protected:
            void initDefault();

        private:
            friend class Singleton<VkGenres>;
            inline VkGenres() : TargetGenres() {
                initDefault();
            }
        };
    }
}

#endif // VK_GENRES
