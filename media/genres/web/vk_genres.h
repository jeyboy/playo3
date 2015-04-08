#ifndef VK_GENRES
#define VK_GENRES

#include "../genres_interface.h"

class VkGenres : public IGenres {
public:
    static VkGenres * instance();
    static void close() {
        delete self;
    }

    int toStandartId(int vkGenreId);
    inline int defaultInt() const { return 18; }
protected:
    void initDefault();

private:
    inline VkGenres() : IGenres() {
        initDefault();
    }

    static VkGenres * self;
};

#endif // VK_GENRES
