#include "mediainfo.h"

MediaInfo::MediaInfo(QUrl uri, bool hasExtension, bool onlyTags) : fileName(0),
    year(-1), track(-1), channels(-1), bitrate(-1), duration(0),
    sampleRate(-1), size(0), error(false), readed(false), remote(!uri.isLocalFile()) {

    if (!remote) {
        QString file_path = uri.toLocalFile();

        // taglib not worked with files without extensions :(
        if (!hasExtension)
            Extensions::instance() -> restoreExtension(file_path, ext);

//        error = !TagLib::FileRef::defaultFileExtensions.contains(TagLib::String(ext.toLower().toStdWString()));

        fileName = new TagLib::FileName(file_path.toStdWString().data());
        TagLib::FileRef f(*fileName, !onlyTags, onlyTags ? TagLib::AudioProperties::Fast : TagLib::AudioProperties::Accurate);

        if (!f.isNull()) {
            readed = true;
            size = f.file() -> length();
            artist = QString::fromStdWString(f.tag() -> artist().toWString());
            title = QString::fromStdWString(f.tag() -> title().toWString());
            album = QString::fromStdWString(f.tag() -> album().toWString());
            setGenre(QString::fromStdWString(f.tag() -> genre().toWString()));
            year = f.tag() -> year();
            track = f.tag() -> track();

            if (!onlyTags)
                readInfo(f);
        } else {
            QFile f(file_path);
            error = f.error() != QFile::NoError;
            size = f.size();
            f.close();
        }
    }
    else if (!onlyTags) {
        readed = true;
        error = !Player::instance() -> getFileInfo(uri, this); // this method only initiating tech info
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
