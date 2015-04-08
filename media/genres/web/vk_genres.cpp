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
    genres.insert("Rock", 1);
    genres.insert("Pop", 2);
    genres.insert("Rap & Hip-Hop", 3);
    genres.insert("Easy Listening", 4);
    genres.insert("Dance & House", 5);
    genres.insert("Instrumental", 6);
    genres.insert("Metal", 7);
    genres.insert("Dubstep", 8);
    genres.insert("Jazz & Blues", 9);
    genres.insert("Drum & Bass", 10);
    genres.insert("Trance", 11);
    genres.insert("Chanson", 12);
    genres.insert("Ethnic", 13);
    genres.insert("Acoustic & Vocal", 14);
    genres.insert("Reggae", 15);
    genres.insert("Classical", 16);
    genres.insert("Indie Pop", 17);
    genres.insert("Other", 18);
    genres.insert("Speech", 19);
    genres.insert("Alternative", 21);
    genres.insert("Electropop & Disco", 22);
}
