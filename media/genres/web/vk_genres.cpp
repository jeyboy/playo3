#include "vk_genres.h"

VkGenres * VkGenres::self = 0;

VkGenres * VkGenres::instance() {
    if(!self)
        self = new VkGenres();
    return self;
}

void VkGenres::initDefault() {
    genres.insert(1, QString("Rock"));
    genres.insert(2, QString("Pop"));
    genres.insert(3, QString("Rap & Hip-Hop"));
    genres.insert(4, QString("Easy Listening"));
    genres.insert(5, QString("Dance & House"));
    genres.insert(6, QString("Instrumental"));
    genres.insert(7, QString("Metal"));
    genres.insert(8, QString("Dubstep"));
    genres.insert(9, QString("Jazz & Blues"));
    genres.insert(10, QString("Drum & Bass"));
    genres.insert(11, QString("Trance"));
    genres.insert(12, QString("Chanson"));
    genres.insert(13, QString("Ethnic"));
    genres.insert(14, QString("Acoustic & Vocal"));
    genres.insert(15, QString("Reggae"));
    genres.insert(16, QString("Classical"));
    genres.insert(17, QString("Indie Pop"));
    genres.insert(18, QString("Other"));
    genres.insert(19, QString("Speech"));
    genres.insert(21, QString("Alternative"));
    genres.insert(22, QString("Electropop & Disco"));
}
