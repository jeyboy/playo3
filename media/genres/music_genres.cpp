#include "music_genres.h"

MusicGenres * MusicGenres::self = 0;

MusicGenres * MusicGenres::instance() {
    if(!self)
        self = new MusicGenres();
    return self;
}

void MusicGenres::initDefault() {
    genres.insert("Blues", 0);
    genres.insert("Classic Rock", 1);
    genres.insert("Country", 2);
    genres.insert("Dance", 3);
    genres.insert("Disco", 4);
    genres.insert("Funk", 5);
    genres.insert("Grunge", 6);
    genres.insert("Hip-Hop", 7);
    genres.insert("HipHop", 7);
    genres.insert("Jazz", 8);
    genres.insert("Metal", 9);
    genres.insert("New Age", 10);
    genres.insert("NewAge", 10);
    genres.insert("Oldies", 11);
    genres.insert("Other", 12);
    genres.insert("Pop", 13);
    genres.insert("Rhythm and Blues", 14);
    genres.insert("Rhythm & Blues", 14);
    genres.insert("Rap", 15);
    genres.insert("Reggae", 16);
    genres.insert("Rock", 17);
    genres.insert("Techno", 18);
    genres.insert("Industrial", 19);
    genres.insert("Alternative", 20);
    genres.insert("Ska", 21);
    genres.insert("Death Metal", 22);
    genres.insert("DeathMetal", 22);
    genres.insert("Pranks", 23);
    genres.insert("Soundtrack", 24);
    genres.insert("Euro-Techno", 25);
    genres.insert("EuroTechno", 25);
    genres.insert("Ambient", 26);
    genres.insert("Trip-Hop", 27);
    genres.insert("TripHop", 27);
    genres.insert("Trip", 27);
    genres.insert("Vocal", 28);
    genres.insert("Jazz&Funk", 29);
    genres.insert("Jazz & Funk", 29);
    genres.insert("Jazz and Funk", 29);
    genres.insert("Fusion", 30);
    genres.insert("Trance", 31);
    genres.insert("Classical", 32);
    genres.insert("Instrumental", 33);
    genres.insert("Acid", 34);
    genres.insert("House", 35);
    genres.insert("Game", 36);
    genres.insert("Sound Clip", 37);
    genres.insert("Gospel", 38);
    genres.insert("Noise", 39);
    genres.insert("Alternative Rock", 40);
    genres.insert("AlternativeRock", 40);
    genres.insert("Bass", 41);
    genres.insert("Soul", 42);
    genres.insert("Punk", 43);
    genres.insert("Space", 44);
    genres.insert("Meditative", 45);
    genres.insert("Instrumental Pop", 46);
    genres.insert("InstrumentalPop", 46);
    genres.insert("Instrumental Rock", 47);
    genres.insert("InstrumentalRock", 47);
    genres.insert("Ethnic", 48);
    genres.insert("Gothic", 49);
    genres.insert("Darkwave", 50);
    genres.insert("Techno-Industrial", 51);
    genres.insert("TechnoIndustrial", 51);
    genres.insert("Techno Industrial", 51);
    genres.insert("Electronic", 52);
    genres.insert("Pop-Folk", 53);
    genres.insert("PopFolk", 53);
    genres.insert("Pop Folk", 53);
    genres.insert("Eurodance", 54);
    genres.insert("Dream", 55);
    genres.insert("Southern Rock", 56);
    genres.insert("Comedy", 57);
    genres.insert("Cult", 58);
    genres.insert("Gangsta", 59);
    genres.insert("Top 40", 60);
    genres.insert("Top40", 60);
    genres.insert("Christian Rap", 61);
    genres.insert("Pop/Funk", 62);
    genres.insert("Pop-Funk", 62);
    genres.insert("PopFunk", 62);
    genres.insert("Jungle", 63);
    genres.insert("Native American", 64);
    genres.insert("Cabaret", 65);
    genres.insert("New Wave", 66);
    genres.insert("NewWave", 66);
    genres.insert("Psychedelic", 67);
    genres.insert("Rave", 68);
    genres.insert("Showtunes", 69);
    genres.insert("Trailer", 70);
    genres.insert("Lo-Fi", 71);
    genres.insert("LoFi", 71);
    genres.insert("Lo Fi", 71);
    genres.insert("Tribal", 72);
    genres.insert("Acid Punk", 73);
    genres.insert("AcidPunk", 73);
    genres.insert("AcidJazz", 74);
    genres.insert("Acid Jazz", 74);
    genres.insert("Polka", 75);
    genres.insert("Retro", 76);
    genres.insert("Musical", 77);
    genres.insert("Rock & Roll", 78);
    genres.insert("Rock&Roll", 78);
    genres.insert("Rock and Roll", 78);
    genres.insert("Hard Rock", 79);
    genres.insert("Folk", 80);
    genres.insert("Folk-Rock", 81);
    genres.insert("Folk Rock", 81);
    genres.insert("FolkRock", 81);
    genres.insert("National Folk", 82);
    genres.insert("Swing", 83);
    genres.insert("Fast Fusion", 84);
    genres.insert("Bebop", 85);
    genres.insert("Latin", 86);
    genres.insert("Revival", 87);
    genres.insert("Celtic", 88);
    genres.insert("Bluegrass", 89);
    genres.insert("Avantgarde", 90);
    genres.insert("Gothic Rock", 91);
    genres.insert("Progressive Rock", 92);
    genres.insert("Psychedelic Rock", 93);
    genres.insert("Symphonic Rock", 94);
    genres.insert("Slow Rock", 95);
    genres.insert("Big Band", 96);
    genres.insert("Chorus", 97);
    genres.insert("Easy Listening", 98);
    genres.insert("Acoustic", 99);
    genres.insert("Humour", 100);
    genres.insert("Speech", 101);
    genres.insert("Chanson", 102);
    genres.insert("Opera", 103);
    genres.insert("Chamber Music", 104);
    genres.insert("Sonata", 105);
    genres.insert("Symphony", 106);
    genres.insert("Booty Bass", 107);
    genres.insert("Primus", 108);
    genres.insert("Porn groove", 109);
    genres.insert("Satire", 110);
    genres.insert("Slow Jam", 111);
    genres.insert("Club", 112);
    genres.insert("Tango", 113);
    genres.insert("Samba", 114);
    genres.insert("Folklore", 115);
    genres.insert("Ballad", 116);
    genres.insert("Power Ballad", 117);
    genres.insert("Rhythmic Soul", 118);
    genres.insert("Freestyle", 119);
    genres.insert("Duet", 120);
    genres.insert("Punk Rock", 121);
    genres.insert("Drum Solo", 122);
    genres.insert("A capella", 123);
    genres.insert("Acapella", 123);
    genres.insert("Euro-House", 124);
    genres.insert("Dance Hall", 125);
    genres.insert("Goa", 126);
    genres.insert("Drum & Bass", 127);
    genres.insert("Drum&Bass", 127);
    genres.insert("Drum and Bass", 127);
    genres.insert("Club-House", 128);
    genres.insert("ClubHouse", 128);
    genres.insert("Club House", 128);
    genres.insert("Hardcore Techno", 129);
    genres.insert("Terror", 130);
    genres.insert("Indie", 131);
    genres.insert("BritPop", 132);
    genres.insert("Afro-punk", 133);
    genres.insert("Afropunk", 133);
    genres.insert("Afro punk", 133);
    genres.insert("Polsk Punk", 134);
    genres.insert("Beat", 135);
    genres.insert("Christian Gangsta Rap", 136);
    genres.insert("Heavy Metal", 137);
    genres.insert("Black Metal", 138);
    genres.insert("Crossover", 139);
    genres.insert("Contemporary Christian", 140);
    genres.insert("Christian Rock", 141);
    genres.insert("Merengue", 142);
    genres.insert("Salsa", 143);
    genres.insert("Thrash Metal", 144);
    genres.insert("Anime", 145);
    genres.insert("Jpop", 146);
    genres.insert("Synthpop", 147);
    genres.insert("Abstract", 148);
    genres.insert("Art Rock", 149);
    genres.insert("Baroque", 150);
    genres.insert("Bhangra", 151);
    genres.insert("Big Beat", 152);
    genres.insert("BigBeat", 152);
    genres.insert("Breakbeat", 153);
    genres.insert("Chillout", 154);
    genres.insert("Downtempo", 155);
    genres.insert("Dub", 156);
    genres.insert("EBM", 157);
    genres.insert("Eclectic", 158);
    genres.insert("Electro", 159);
    genres.insert("Electroclash", 160);
    genres.insert("Emo", 161);
    genres.insert("Experimental", 162);
    genres.insert("Garage", 163);
    genres.insert("Global", 164);
    genres.insert("IDM", 165);
    genres.insert("Illbient", 166);
    genres.insert("Industro-Goth", 167);
    genres.insert("Jam Band", 168);
    genres.insert("Krautrock", 169);
    genres.insert("Leftfield", 170);
    genres.insert("Lounge", 171);
    genres.insert("Math Rock", 172);
    genres.insert("New Romantic", 173);
    genres.insert("Nu-Breakz", 174);
    genres.insert("Nu Breakz", 174);
    genres.insert("NuBreakz", 174);
    genres.insert("Post-Punk", 175);
    genres.insert("PostPunk", 175);
    genres.insert("Post Punk", 175);
    genres.insert("Post-Rock", 176);
    genres.insert("Post Rock", 176);
    genres.insert("PostRock", 176);
    genres.insert("Psytrance", 177);
    genres.insert("Shoegaze", 178);
    genres.insert("Space Rock", 179);
    genres.insert("Trop Rock", 180);
    genres.insert("World Music", 181);
    genres.insert("Neoclassical", 182);
    genres.insert("Audiobook", 183);
    genres.insert("Audio Theatre", 184);
    genres.insert("Neue Deutsche Welle", 185);
    genres.insert("Podcast", 186);
    genres.insert("Indie Rock", 187);
    genres.insert("G-Funk", 188);
    genres.insert("G Funk", 188);
    genres.insert("GFunk", 188);
    genres.insert("Dubstep", 189);
    genres.insert("Garage Rock", 190);
    genres.insert("Psybient", 191);
}
