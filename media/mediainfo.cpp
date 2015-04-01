#include "mediainfo.h"
#include <qdebug.h>

MediaInfo::MediaInfo(QUrl uri, bool onlyTags) :
    year(-1), track(-1), channels(-1), bitrate(-1), duration(0),
    sampleRate(-1), size(0), readed(false), remote(!uri.isLocalFile()) {

    if (!remote) {
        fileName = new TagLib::FileName(uri.toLocalFile().toStdWString().data());
        TagLib::FileRef f(*fileName, !onlyTags, onlyTags ? TagLib::AudioProperties::Fast : TagLib::AudioProperties::Accurate);

        if (!f.isNull()) {
            readed = true;
            size = f.file() -> length();
            artist = QString::fromStdWString(f.tag() -> artist().toWString());
            title = QString::fromStdWString(f.tag() -> title().toWString());
            album = QString::fromStdWString(f.tag() -> album().toWString());
            genre = QString::fromStdWString(f.tag() -> genre().toWString());
            year = f.tag() -> year();
            track = f.tag() -> track();

            if (!onlyTags)
                readInfo(f);
        } else {
            QFile f(uri.toLocalFile());
            size = f.size();
            f.close();
        }
    }
    else if (!onlyTags) {
        readed = true;
        Player::instance() -> getFileInfo(uri, this); // this method only initiating tech info
    }
}

void MediaInfo::initInfo() {
    TagLib::FileRef f(*fileName, true, TagLib::AudioProperties::Accurate);
    readInfo(f);
}

void MediaInfo::readInfo(TagLib::FileRef f) {
    channels = f.audioProperties() -> channels();
    duration = f.audioProperties() -> length();
    sampleRate = f.audioProperties() -> sampleRate();

    bitrate = f.audioProperties() -> bitrate();
    if (bitrate == 0 && duration > 0)
        bitrate = calcAverageBitrate();
}
