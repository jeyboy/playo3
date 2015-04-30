#include "vk_genres.h"

VkGenres * VkGenres::self = 0;

VkGenres * VkGenres::instance() {
    if(!self)
        self = new VkGenres();
    return self;
}

int VkGenres::fromStandartId(int genreId) {//TODO: need to finish this
    switch(genreId) {
//        case 1: return 17;
        default: return defaultInt();
    }
}

int VkGenres::toStandartId(int vkGenreId) {
    switch(vkGenreId) {
        case 1: return 17;
        case 2: return 13;
        case 3: return 15;
        case 4: return 98;
        case 5: return 3;
        case 6: return 33;
        case 7: return 9;
        case 8: return 189;
        case 9: return 0;
        case 10: return 127;
        case 11: return 31;
        case 12: return 102;
        case 13: return 48;
        case 14: return 99;
        case 15: return 16;
        case 16: return 32;
        case 17: return 131;
        case 18: return 12;
        case 19: return 101;
        case 21: return 20;
        case 22: return 4;
        default: return defaultInt();
    }
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
