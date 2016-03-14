#include "mediainfo.h"
#include "player/player_index.h"

using namespace Core::Media;

MediaInfo::MediaInfo(const QUrl & uri, const QVariant & extension, bool onlyTags) : IMediaInfo(!uri.isLocalFile()), fileName(0) {
    if (extension.isValid())
        ext = extension.toString();

    if (!remote) {
        file_path = uri.toLocalFile();

        // taglib not worked with files without extensions :(
        if (!extension.isValid())
            Extensions::obj().restoreExtension(file_path, ext);

//        error = !TagLib::FileRef::defaultFileExtensions.contains(TagLib::String(ext.toLower().toStdWString()));

        #ifdef Q_OS_WIN
            fileName = new TagLib::FileName(file_path.toStdWString().data());
        #else
            fileName = new TagLib::FileName(QFile::encodeName(file_path).toStdString().data());
        #endif
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
        error = !PlayerFactory::obj().currPlayer() -> fileInfo(uri, this); // this method only initiating tech info
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
