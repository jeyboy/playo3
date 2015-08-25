#include "music_genres.h"

MusicGenres * MusicGenres::self = 0;

MusicGenres * MusicGenres::instance() {
    if(!self)
        self = new MusicGenres();
    return self;
}

void MusicGenres::load() {
    settings = new Playo3::DataStore(GENRES_PATH);

    QJsonArray ar = settings -> read(ROOT_VAL_NAME).toArray();
    for(QJsonArray::Iterator genre_obj = ar.begin(); genre_obj != ar.end(); genre_obj++) {
        QJsonObject genre = (*genre_obj).toObject();
        genres.insert(genre.value(GENRE_VAL_NAME).toString(), genre.value(GENRE_NUM_NAME).toInt());
    }
}

void MusicGenres::save() {
    settings -> clear();

    QJsonArray ar;

    for(QHash<QString, int>::Iterator genre = genres.begin(); genre != genres.end(); genre++) {
        QJsonObject obj;
        obj.insert(GENRE_VAL_NAME, genre.key());
        obj.insert(GENRE_NUM_NAME, genre.value());
        ar << obj;
    }

    settings -> write(ROOT_VAL_NAME, ar);
}

//https://en.wikipedia.org/wiki/List_of_music_styles
//http://everynoise.com/everynoise1d.cgi?root=singer-songwriter&scope=all

void MusicGenres::prepare() {
    // spotify relation there
}
