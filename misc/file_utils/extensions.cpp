#include "extensions.h"
#include <qdebug.h>

using namespace Playo3;

Extensions * Extensions::self = 0;

Extensions * Extensions::instance() {
    if(!self)
        self = new Extensions();
    return self;
}

Extensions::Extensions() {
    ext = new DataStore("extensions.json");

    if (ext -> state) {
        activeFilter = ext -> read("active").toString("all");
        QJsonObject obj = ext -> read("filters").toObject();

        foreach (QString key, obj.keys())
            filters.insert(key, obj.value(key).toVariant().value<QStringList>());

    } else {
        QStringList commonfiltersList;
        commonfiltersList << "*";
        filters.insert("all", commonfiltersList);
        activeFilter = "all";

        initMusicExtensions();
    }

    initSignatures();
}

void Extensions::initSignatures() {
    ext_signatures.insert("6d6f6f76", "mov");                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert("6d6f6f76", 4);

    ext_signatures.insert("66726565", "mov");                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert("66726565", 4);

    ext_signatures.insert("6d646174", "mov");                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert("6d646174", 4);

    ext_signatures.insert("77696465", "mov");                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert("77696465", 4);

    ext_signatures.insert("706e6f74", "mov");                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert("706e6f74", 4);

    ext_signatures.insert("736b6970", "mov");                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert("736b6970", 4);

    ext_signatures.insert("0000001466747970", "3gp");                       // 3GPP multimedia files
    ext_signatures.insert("0000002066747970", "3gp");                       // 3GPP2 multimedia files
    ext_signatures.insert("0000001866747970", "3gp5");                      // MPEG-4 video files
    ext_signatures.insert("464f524d00", "aiff");                            // Audio Interchange File
    ext_signatures.insert("3026b2758e66cf11", "asf" /*"wma" "wmv"*/);       // Windows Media Audio|Video File
    ext_signatures.insert("a6d900aa0062ce6c", "asf" /*"wma" "wmv"*/);       // Windows Media Audio|Video File
    ext_signatures.insert("5343486c", "ast");                               // Underground Audio
    ext_signatures.insert("52494646", "dat" /*"4xm" "avi" "wav"*/);         // Video CD MPEG movie
    ext_signatures.insert("574d4d50", "dat");                               // Walkman MP3 file
    ext_signatures.insert("664c6143", "flac");                              // Free Lossless Audio Codec file
    ext_signatures.insert("4d4143", "ape");                                 // Monkeys Lossless Audio Codec file
    ext_signatures.insert("464c56", "flv");                                 // Flash video file
    ext_signatures.insert("445644", "ifo");                                 // DVD info file
    ext_signatures.insert("2e524543", "ivr");                               // RealPlayer video file (V11+)
    ext_signatures.insert("00000020667479704d3441", "m4a");                 // Apple audio and video files
    ext_signatures.insert("4d546864", "midi");                              // MIDI sound file
    ext_signatures.insert("1a45dfa393428288", "mkv");                       // Matroska stream file
    ext_signatures.insert("494433", "mp3");                                 // MP3 audio file
    ext_signatures.insert("fffb", "mp3");                                   // MP3 audio file
    ext_signatures.insert("000001ba", "mpg" /*"vob"*/);                     // DVD video file
    ext_signatures.insert("000001b3", "mpg");                               // MPEG video file
    ext_signatures.insert("4f67675300020000", "ogg" /*"oga" "ogv" "ogx"*/); // Ogg Vorbis Codec compressed file
    ext_signatures.insert("2e524d4600000012", "ra");                        // RealAudio file
    ext_signatures.insert("2e7261fd00", "ra");                              // RealAudio streaming media
    ext_signatures.insert("2e524d46", "rm" /*rmvb*/);                       // RealMedia streaming media

    ext_signatures.insert("646e732e", "au");                                // Audacity audio file

    ext_signatures.insert("47494638", "gif");                               // GIF file
    ext_signatures.insert("424d", "bmp" /*"dib"*/);                         // Bitmap image
    ext_signatures.insert("b168de3a", "dcx");                               // PCX bitmap
    ext_signatures.insert("7801730d626260", "dmg");                         // MacOS X image file
    ext_signatures.insert("ffd8ffe0", "jfif" /*"jpg" "jpe" "jpeg"*/);       // JPEG IMAGE
    ext_signatures.insert("0ced", "mp");                                    // Monochrome Picture TIFF bitmap
    ext_signatures.insert("89504e470d0a1a0a", "png");                       // PNG image
    ext_signatures.insert("492049", "tif");                                 // TIFF file
    ext_signatures.insert("49492a00", "tif");                               // TIFF file
    ext_signatures.insert("4d4d002a", "tif");                               // TIFF file
    ext_signatures.insert("4d4d002b", "tif");                               // TIFF file

    ext_signatures.insert("25504446", "pdf" /*"fdf"*/);                     // PDF file
    ext_signatures.insert("dba52d00", "doc");                               // Word 2.0 file
    ext_signatures.insert("504b030414000600", "docx" /*"pptx" "xlsx"*/);    // MS Office 2007 documents
    ext_signatures.insert("d0cf11e0a1b11ae1", "ppt" /*"pps"*/);             // Microsoft Office document
    ext_signatures.insert("4d534346", "ppz");                               // Powerpoint Packaged Presentation

    ext_signatures.insert("377a bc af 27 1c", "7z");                        // 7-Zip compressed file
    ext_signatures.insert("4172 43 01", "arc");                             // FreeArc compressed file
    ext_signatures.insert("1a02", "arc");                                   // LH archive (old vers.|type 1)
    ext_signatures.insert("1a03", "arc");                                   // LH archive (old vers.|type 2)
    ext_signatures.insert("1a04", "arc");                                   // LH archive (old vers.|type 3)
    ext_signatures.insert("1a08", "arc");                                   // LH archive (old vers.|type 4)
    ext_signatures.insert("1a09", "arc");                                   // LH archive (old vers.|type 5)
    ext_signatures.insert("60ea", "arj");                                   // ARJ Compressed archive file
    ext_signatures.insert("425a68", "bz2");                                 // bzip2 compressed archive
    ext_signatures.insert("1f8b08", "gz");                                  // GZIP archive file
    ext_signatures.insert("5f27a889", "jar");                               // Jar archive
    ext_signatures.insert("4a4152435300", "jar");                           // JARCS compressed archive
    ext_signatures.insert("526172211a07", "rar");                           // RAR archive
    ext_signatures.insert("504b0506", "zip");                               // PKZIP archive
    ext_signatures.insert("504b0708", "zip");                               // PKZIP archive
    ext_signatures.insert("5a4f4f20", "zoo");                               // ZOO compressed archive

    ext_signatures.insert("4d5a", "exe");                                   // Executable
}

QString Extensions::folderName(QFileInfo & info) {
    QString name;
    if (info.isDir())
        name = info.filePath();
    else
        name = info.dir().path();
    return name.split('/').last();
}

bool Extensions::restoreExtension(QString & file_path, QString & restoredExt) {
    QFile f(file_path);
    bool finded = false;

    if (f.open(QIODevice::ReadOnly)) {
        QByteArray arHex = f.read(11).toHex();
        QString sH(arHex);

        QHash<QString, QString>::Iterator it = ext_signatures.begin();

        for(; it != ext_signatures.end(); it++) {
            if (sH.mid(ext_signature_offset.value(it.key(), 0) * 2, it.key().size()) == it.key()) {
                if ((finded = f.rename(file_path + "." + it.value())))
                    file_path += "." + (restoredExt = it.value());
                break;
            }
        }

        f.close();
    }

    return finded;
}

//TODO: try to find join file with folder search pattern

QFileInfoList Extensions::folderFiles(QFileInfo file) {
    return QDir(file.filePath()).entryInfoList(activeFilterList(), QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
}

QFileInfoList Extensions::folderDirectories(QFileInfo file) {
    return QDir(file.filePath()).entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
//    return QDir(file.filePath()).entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden);
}

bool Extensions::respondToExtension(QString ext) const {
    return activeFilterList().contains("*." + ext) || activeFilterList().contains("*.*") || activeFilterList().contains("*");
}

QStringList Extensions::activeFilterList() const {
    return filterList(activeFilter);
}

QStringList Extensions::filterList(const QString & name) const {
    if (filters.isEmpty())
        return QStringList();
    else
        return filters.value(name, filters.values().first());
}

void Extensions::filterListUpdate(const QString & name, QStringList & newList) {
    filters.insert(name, newList);
}

void Extensions::addNewPreset(QString name) {
    filters.insert(name, QStringList());
}

void Extensions::removePreset(QString name) {
    filters.remove(name);
    if (name == activeFilter)
        activeFilter = filters.keys().first();
}

void Extensions::initMusicExtensions() { // TODO: not complete list
    QStringList mList;
    mList << "*.aif";   // Audio Interchange File
    mList << "*.aifc";  // Audio Interchange File
    mList << "*.aiff";  // Audio Interchange File
    mList << "*.asf";   // Windows Media formats
    mList << "*.adt";   // Windows Media formats
    mList << "*.adts";  // Windows Media formats
    mList << "*.cda";   // CD Audio track
    mList << "*.flac";  // Audio files encoded by Flac - free lossless audio codec
    mList << "*.fla";   // Audio files encoded by Flac - free lossless audio codec
    mList << "*.mp2";   // Compressed audio and music file
    mList << "*.mpa";   // Compressed audio and music file
    mList << "*.mp3";   // Compressed audio and music file, mp3 songs or ringtones
    mList << "*.amr";   // Adaptive Multi-Rate compressed audio file
    mList << "*.wv";    // WavPack lossless compressed audio file
    mList << "*.ram";   // Real Audio metafile
    mList << "*.m4a";   // MPEG-4 compressed audio file
    mList << "*.wav";   // WAVe PCM Sound, standard Windows sound format
    mList << "*.ad4";   // 4D Systems audio file
    mList << "*.thd";   // Dolby TrueHD audio file
    mList << "*.wma";   // Windows Media Audio
    mList << "*.aac";   // Advanced audio coding file
    mList << "*.m4r";   // Apple iPhone ringtone format
    mList << "*.mogg";  // Multitrack digital audio format
    mList << "*.spx";   // Speex encoded audio
    mList << "*.ogg";   // Ogg Vorbis audio file
    mList << "*.au";    // Unix audio sound file format
    mList << "*.pcm";   // PCM audio file
    mList << "*.m4p";   // Apple iTunes Music Store audio file
    mList << "*.mka";   // Matroska audio file
    mList << "*.snd";   // Sound file
    mList << "*.swa";   // Adobe Shockwave music file
    mList << "*.aob";   // DVD Audio Object file
    mList << "*.rms";   // RealMedia Secure Media file format
    mList << "*.m4b";   // MPEG-4 Audio Layer audio book file
    mList << "*.acm";   // ACM compressed sound file

    filters.insert("music", mList);
}
