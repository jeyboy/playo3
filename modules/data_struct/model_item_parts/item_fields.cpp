#include "item_fields.h"

using namespace Playo3;

ItemFields::ItemFields(QJsonObject * hash) : ItemState(hash -> value(JSON_TYPE_STATE).toInt()) {
    _path = hash -> value(JSON_TYPE_PATH).toString();
    _title = hash -> value(JSON_TYPE_TITLE).toString();
    _extension = hash -> value(JSON_TYPE_EXTENSION).toString();
    _genreID = hash -> value(JSON_TYPE_GENRE_ID).toInt(-1);
    _duration = hash -> value(JSON_TYPE_DURATION).toString("");

    _size = hash -> value(JSON_TYPE_BYTES_SIZE).toInt(-1);
    _info = hash -> value(JSON_TYPE_INFO).toString("");
    _bpm = hash -> value(JSON_TYPE_BPM).toInt(0);
}

ItemFields::ItemFields(QString path, QString title, QString extension, int size, int initState) : ItemState(initState) {
    _path = path;
    _title = title;
    _extension = extension;
    _genreID = -1;
    _duration = "";

    _size = size;
    _info = "";
    _bpm = 0;
}

ItemFields::ItemFields(int state) : ItemState(state) {
    _path = "";
    _title = "";
    _extension = "";
    _genreID = -1;
    _duration = "";

    _size = -1;
    _info = "";
    _bpm = 0;
}


QJsonObject ItemFields::toJson() {
    QJsonObject root = QJsonObject();

    root[JSON_TYPE_TITLE] = _title;
    root[JSON_TYPE_STATE] = saveStateVal();
    root[JSON_TYPE_PATH] = _path;

    if (!_info.isEmpty())
        root[JSON_TYPE_INFO] = _info;

    if (_bpm != 0)
        root[JSON_TYPE_BPM] = _bpm;

    if (_size != -1)
        root[JSON_TYPE_BYTES_SIZE] = _size;

    if (_genreID != -1)
        root[JSON_TYPE_GENRE_ID] = _genreID;

    if (!_duration.isEmpty())
        root[JSON_TYPE_DURATION] = _duration;

    if (!_extension.isNull())
        root[JSON_TYPE_EXTENSION] = _extension;

    return root;
}

QString ItemFields::downloadTitle() const {
    QString ret = filenameFilter(_title);

    if (!_extension.isEmpty())
        ret = ret + '.' + _extension;

    return ret;
}
