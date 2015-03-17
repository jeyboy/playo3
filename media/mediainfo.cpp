#include "mediainfo.h"

MediaInfo::MediaInfo(QString filepath, bool onlyTags) {
    readed = false;
    fileName = filepath.toStdString();
    TagLib::FileRef f(fileName.c_str(), !onlyTags, onlyTags ? TagLib::AudioProperties::Fast : TagLib::AudioProperties::Accurate);

    if (!f.isNull()) {
        size = f.file() -> length();
        readed = true;
        artist = QString::fromStdWString(f.tag() -> artist().toWString());
        title = QString::fromStdWString(f.tag() -> title().toWString());
        album = QString::fromStdWString(f.tag() -> album().toWString());
        genre = QString::fromStdWString(f.tag() -> genre().toWString());
        year = f.tag() -> year();
        track = f.tag() -> track();

        if (!onlyTags)
            readInfo(f);
    }
}

void MediaInfo::initInfo() {
    TagLib::FileRef f(fileName.c_str(), true, TagLib::AudioProperties::Accurate);
    readInfo(f);
}

void MediaInfo::readInfo(TagLib::FileRef f) {
    bitrate = f.audioProperties() -> bitrate();
    channels = f.audioProperties() -> channels();
    duration = f.audioProperties() -> length();
    sampleRate = f.audioProperties() -> sampleRate();
}

bool MediaInfo::initiated() const {
    return readed;
}

QString MediaInfo::getArtist() const { return artist; }
QString MediaInfo::getTitle() const { return title; }
QString MediaInfo::getAlbum() const { return album; }
QString MediaInfo::getGenre() const { return genre; }

long MediaInfo::getSize() const { return size; }
int MediaInfo::getYear() const { return year; }
int MediaInfo::getTrack() const { return track; }
int MediaInfo::getChannels() const { return channels; }
int MediaInfo::getBitrate() const { return bitrate; }
int MediaInfo::getDuration() const { return duration; }
int MediaInfo::getSampleRate() const { return sampleRate; }
