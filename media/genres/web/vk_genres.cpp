#include "vk_genres.h"

VkGenres * VkGenres::self = 0;

VkGenres * VkGenres::instance() {
    if(!self)
        self = new VkGenres();
    return self;
}

int VkGenres::fromStandartId(int genreId) {
    switch(genreId) {
        case 0: return 9;
        case 3: return 5;
        case 4: return 22;
        case 7: return 3;
        case 8: return 9;
        case 9: return 7;
        case 12: return 18;
        case 13: return 2;
        case 14: return 9;
        case 15: return 3;
        case 16: return 15;
        case 17: return 1;
        case 20: return 21;
        case 22: return 7;
        case 28: return 14;
        case 29: return 9;
        case 31: return 11;
        case 32: return 16;
        case 33: return 6;
        case 35: return 5;
        case 40: return 1;
        case 46: return 2;
        case 47: return 1;
        case 48: return 13;
        case 53: return 2;
        case 54: return 5;
        case 56: return 1;
        case 61: return 3;
        case 62: return 2;
        case 74: return 9;
        case 79: return 1;
        case 81: return 1;
        case 91: return 1;
        case 92: return 1;
        case 93: return 1;
        case 94: return 1;
        case 95: return 1;
        case 98: return 4;
        case 99: return 14;
        case 101: return 19;
        case 102: return 12;
        case 120: return 14;
        case 121: return 1;
        case 124: return 5;
        case 125: return 5;
        case 127: return 10;
        case 128: return 5;
        case 131: return 17;
        case 132: return 2;
        case 136: return 3;
        case 137: return 7;
        case 138: return 7;
        case 141: return 1;
        case 144: return 7;
        case 146: return 2;
        case 147: return 2;
        case 149: return 1;
        case 159: return 22;
        case 172: return 1;
        case 176: return 1;
        case 177: return 11;
        case 179: return 1;
        case 180: return 1;
        case 182: return 16;
        case 187: return 1;
        case 189: return 8;
        case 190: return 1;

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

QList<int> VkGenres::toStandartIds(int vkGenreId) {
    QList<int> res;

    switch(vkGenreId) {
        case 1: {
            res.append(toStandartId(vkGenreId));
            res.append(40);
            res.append(47); res.append(56);
            res.append(79); res.append(81);
            res.append(91); res.append(92);
            res.append(93); res.append(94);
            res.append(95); res.append(121);
            res.append(141); res.append(149);
            res.append(172); res.append(176);
            res.append(179); res.append(180);
            res.append(187); res.append(190);
        break;}
        case 2: {
            res.append(toStandartId(vkGenreId));
            res.append(46); res.append(53);
            res.append(62); res.append(132);
            res.append(146); res.append(147);
        break;}
        case 3: {
            res.append(toStandartId(vkGenreId));
            res.append(61); res.append(136);
            res.append(7);
        break;}
        case 4: {
            res.append(toStandartId(vkGenreId));
        break;}
        case 5: {
            res.append(toStandartId(vkGenreId));
            res.append(54); res.append(125);
            res.append(35); res.append(124);
            res.append(128);
        break;}
        case 6: {
            res.append(toStandartId(vkGenreId));
//            res.append(46); res.append(47);
        break;}
        case 7: {
            res.append(toStandartId(vkGenreId));
            res.append(22); res.append(137);
            res.append(138); res.append(144);
        break;}
        case 8: {
            res.append(toStandartId(vkGenreId));
        break;}
        case 9: {
            res.append(toStandartId(vkGenreId));
            res.append(14); res.append(8);
            res.append(29); res.append(74);
        break;}
        case 10: {
            res.append(toStandartId(vkGenreId));
        break;}
        case 11: {
            res.append(toStandartId(vkGenreId));
            res.append(177);
        break;}
        case 12: {
            res.append(toStandartId(vkGenreId));
        break;}
        case 13: {
            res.append(toStandartId(vkGenreId));
        break;}
        case 14: {
            res.append(toStandartId(vkGenreId));
            res.append(28); res.append(120);
        break;}
        case 15: {
            res.append(toStandartId(vkGenreId));
        break;}
        case 16: {
            res.append(toStandartId(vkGenreId));
            res.append(182);
        break;}
        case 17: {
            res.append(toStandartId(vkGenreId));
        break;}
        case 18: {
            res.append(toStandartId(vkGenreId));
        break;}
        case 19: {
            res.append(toStandartId(vkGenreId));
        break;}
        case 21: {
            res.append(toStandartId(vkGenreId));
//            res.append(40);
        break;}
        case 22: {
            res.append(toStandartId(vkGenreId));
            res.append(159);
        break;}
        default: {
            res.append(defaultInt());
        break;}
    }

    return res;
}

void VkGenres::initDefault() {
    genres.insert(QStringLiteral("Rock"), 1);
    genres.insert(QStringLiteral("Pop"), 2);
    genres.insert(QStringLiteral("Rap & Hip-Hop"), 3);
    genres.insert(QStringLiteral("Easy Listening"), 4);
    genres.insert(QStringLiteral("Dance & House"), 5);
    genres.insert(QStringLiteral("Instrumental"), 6);
    genres.insert(QStringLiteral("Metal"), 7);
    genres.insert(QStringLiteral("Dubstep"), 8);
    genres.insert(QStringLiteral("Jazz & Blues"), 9);
    genres.insert(QStringLiteral("Drum & Bass"), 10);
    genres.insert(QStringLiteral("Trance"), 11);
    genres.insert(QStringLiteral("Chanson"), 12);
    genres.insert(QStringLiteral("Ethnic"), 13);
    genres.insert(QStringLiteral("Acoustic & Vocal"), 14);
    genres.insert(QStringLiteral("Reggae"), 15);
    genres.insert(QStringLiteral("Classical"), 16);
    genres.insert(QStringLiteral("Indie Pop"), 17);
    genres.insert(QStringLiteral("Other"), 18);
    genres.insert(QStringLiteral("Speech"), 19);
    genres.insert(QStringLiteral("Alternative"), 21);
    genres.insert(QStringLiteral("Electropop & Disco"), 22);
}
