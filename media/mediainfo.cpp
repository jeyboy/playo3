#include "mediainfo.h"

MediaInfo::MediaInfo(QUrl uri, bool onlyTags) {
    readed = false;
    if (uri.isLocalFile()) {
        fileName = uri.toLocalFile().toStdString();
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
    else Player::instance() -> getFileInfo(uri, this);
}

void MediaInfo::initInfo() {
    TagLib::FileRef f(fileName.c_str(), true, TagLib::AudioProperties::Accurate);
    readInfo(f);
}

void MediaInfo::readInfo(TagLib::FileRef f) {   
    channels = f.audioProperties() -> channels();
    duration = f.audioProperties() -> length();
    sampleRate = f.audioProperties() -> sampleRate();

    bitrate = f.audioProperties() -> bitrate();
    if (bitrate == 0)
        bitrate = calcAverageBitrate();
}
