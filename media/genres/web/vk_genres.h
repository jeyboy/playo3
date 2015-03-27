#ifndef VK_GENRES
#define VK_GENRES

#include "../genres_interface.h"

class VkGenres : public IGenres {
public:
    static VkGenres * instance();
    static void close() {
        delete self;
    }
private:
    inline int defaultInt() const { return 18; }
    void initDefault();

    inline VkGenres() : IGenres() {}

    VkGenres * self;
};

#endif // VK_GENRES
