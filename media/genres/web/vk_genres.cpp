#include "vk_genres.h"
#include "../music_genres.h"

VkGenres * VkGenres::self = 0;

VkGenres * VkGenres::instance() {
    if(!self)
        self = new VkGenres();
    return self;
}

int VkGenres::toStandartId(int vkGenreId) {
    return MusicGenres::instance() -> toInt(genres.value(vkGenreId));
}

void VkGenres::initDefault() {
    genres.insert(1, "Rock");
    genres.insert(2, "Pop");
    genres.insert(3, "Rap & Hip-Hop");
    genres.insert(4, "Easy Listening");
    genres.insert(5, "Dance & House");
    genres.insert(6, "Instrumental");
    genres.insert(7, "Metal");
    genres.insert(8, "Dubstep");
    genres.insert(9, "Jazz & Blues");
    genres.insert(10, "Drum & Bass");
    genres.insert(11, "Trance");
    genres.insert(12, "Chanson");
    genres.insert(13, "Ethnic");
    genres.insert(14, "Acoustic & Vocal");
    genres.insert(15, "Reggae");
    genres.insert(16, "Classical");
    genres.insert(17, "Indie Pop");
    genres.insert(18, "Other");
    genres.insert(19, "Speech");
    genres.insert(21, "Alternative");
    genres.insert(22, "Electropop & Disco");
}
