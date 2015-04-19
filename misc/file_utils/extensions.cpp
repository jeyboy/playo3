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
        foreach (QString key, obj.keys()) {
            filters.insert(key, obj.value(key).toVariant().value<QStringList>());
        }
    } else {
        QStringList commonfiltersList;
        commonfiltersList << "*";
        filters.insert("all", commonfiltersList);
        activeFilter = "all";
    }

    initSignatures();
}

void Extensions::initSignatures() {
    ext_signatures.insert("6d 6f 6f 76", "mov");                                // QuickTime movie // offset: 4 bytes
    ext_signatures.insert("66 72 65 65", "mov");                                // QuickTime movie // offset: 4 bytes
    ext_signatures.insert("6d 64 61 74", "mov");                                // QuickTime movie // offset: 4 bytes
    ext_signatures.insert("77 69 64 65", "mov");                                // QuickTime movie // offset: 4 bytes
    ext_signatures.insert("70 6e 6f 74", "mov");                                // QuickTime movie // offset: 4 bytes
    ext_signatures.insert("73 6b 69 70", "mov");                                // QuickTime movie // offset: 4 bytes

    ext_signatures.insert("00 00 00 14 66 74 79 70", "3gp");                    // 3GPP multimedia files
    ext_signatures.insert("00 00 00 20 66 74 79 70", "3gp");                    // 3GPP2 multimedia files
    ext_signatures.insert("00 00 00 18 66 74 79 70", "3gp5");                   // MPEG-4 video files
    ext_signatures.insert("46 4f 52 4d 00", "aiff");                            // Audio Interchange File
    ext_signatures.insert("30 26 b2 75 8e 66 cf 11", "asf" /*"wma" "wmv"*/);    // Windows Media Audio|Video File
    ext_signatures.insert("a6 d9 00 aa 00 62 ce 6c", "asf" /*"wma" "wmv"*/);    // Windows Media Audio|Video File
    ext_signatures.insert("53 43 48 6c", "ast");                                // Underground Audio
    ext_signatures.insert("64 6e 73 2e", "au");                                 // Audacity audio file
    ext_signatures.insert("52 49 46 46", "dat" /*"4xm" "avi" "wav"*/);          // Video CD MPEG movie
    ext_signatures.insert("57 4d 4d 50", "dat");                                // Walkman MP3 file
    ext_signatures.insert("66 4c 61 43", "flac");                               // Free Lossless Audio Codec file
    ext_signatures.insert("46 4c 56", "flv");                                   // Flash video file
    ext_signatures.insert("44 56 44", "ifo");                                   // DVD info file
    ext_signatures.insert("2e 52 45 43", "ivr");                                // RealPlayer video file (V11+)
    ext_signatures.insert("00 00 00 20 66 74 79 70 4d 34 41", "m4a");           // Apple audio and video files
    ext_signatures.insert("4d 54 68 64", "midi");                               // MIDI sound file
    ext_signatures.insert("1a 45 df a3 93 42 82 88", "mkv");                    // Matroska stream file
    ext_signatures.insert("49 44 33", "mp3");                                   // MP3 audio file
    ext_signatures.insert("ff fb", "mp3");                                      // MP3 audio file
    ext_signatures.insert("00 00 01 ba", "mpg" /*"vob"*/);                      // DVD video file
    ext_signatures.insert("00 00 01 b3", "mpg");                                // MPEG video file
    ext_signatures.insert("4f 67 67 53 00 02 00 00", "ogg" /*"oga" "ogv" "ogx"*/); // Ogg Vorbis Codec compressed file
    ext_signatures.insert("2e 52 4d 46 00 00 00 12", "ra");                     // RealAudio file
    ext_signatures.insert("2e 72 61 fd 00", "ra");                              // RealAudio streaming media
    ext_signatures.insert("2e 52 4d 46", "rm" /*rmvb*/);                        // RealMedia streaming media

    ext_signatures.insert("47 49 46 38", "gif");                                // GIF file
    ext_signatures.insert("42 4d", "bmp" /*"dib"*/);                            // Bitmap image
    ext_signatures.insert("b1 68 de 3a", "dcx");                                // PCX bitmap
    ext_signatures.insert("78 01 73 0d 62 62 60", "dmg");                       // MacOS X image file
    ext_signatures.insert("ff d8 ff e0", "jfif" /*"jpg" "jpe" "jpeg"*/);        // JPEG IMAGE
    ext_signatures.insert("0c ed", "mp");                                       // Monochrome Picture TIFF bitmap
    ext_signatures.insert("89 50 4e 47 0d 0a 1a 0a", "png");                    // PNG image
    ext_signatures.insert("49 20 49", "tif");                                   // TIFF file
    ext_signatures.insert("49 49 2a 00", "tif");                                // TIFF file
    ext_signatures.insert("4d 4d 00 2a", "tif");                                // TIFF file
    ext_signatures.insert("4d 4d 00 2b", "tif");                                // TIFF file

    ext_signatures.insert("25 50 44 46", "pdf" /*"fdf"*/);                      // PDF file
    ext_signatures.insert("db a5 2d 00", "doc");                                // Word 2.0 file
    ext_signatures.insert("50 4b 03 04 14 00 06 00", "docx" /*"pptx" "xlsx"*/); // MS Office 2007 documents
    ext_signatures.insert("d0 cf 11 e0 a1 b1 1a e1", "ppt" /*"pps"*/);          // Microsoft Office document
    ext_signatures.insert("4d 53 43 46", "ppz");                                // Powerpoint Packaged Presentation

    ext_signatures.insert("37 7a bc af 27 1c", "7z");                           // 7-Zip compressed file
    ext_signatures.insert("41 72 43 01", "arc");                                // FreeArc compressed file
    ext_signatures.insert("1a 02", "arc");                                      // LH archive (old vers.|type 1)
    ext_signatures.insert("1a 03", "arc");                                      // LH archive (old vers.|type 2)
    ext_signatures.insert("1a 04", "arc");                                      // LH archive (old vers.|type 3)
    ext_signatures.insert("1a 08", "arc");                                      // LH archive (old vers.|type 4)
    ext_signatures.insert("1a 09", "arc");                                      // LH archive (old vers.|type 5)
    ext_signatures.insert("60 ea", "arj");                                      // ARJ Compressed archive file
    ext_signatures.insert("42 5a 68", "bz2");                                   // bzip2 compressed archive
    ext_signatures.insert("1f 8b 08", "gz");                                    // GZIP archive file
    ext_signatures.insert("5f 27 a8 89", "jar");                                // Jar archive
    ext_signatures.insert("4a 41 52 43 53 00", "jar");                          // JARCS compressed archive
    ext_signatures.insert("52 61 72 21 1a 07", "rar");                          // RAR archive
    ext_signatures.insert("50 4b 05 06", "zip");                                // PKZIP archive
    ext_signatures.insert("50 4b 07 08", "zip");                                // PKZIP archive
    ext_signatures.insert("5a 4f 4f 20", "zoo");                                // ZOO compressed archive

    ext_signatures.insert("4d 5a", "exe");                                      // Executable
}

QString Extensions::folderName(QFileInfo & info) {
    QString name;
    if (info.isDir())
        name = info.filePath();
    else
        name = info.dir().path();
    return name.split('/').last();
}

bool Extensions::restoreExtension(QString & file_path) {
    QFile f(file_path);

    if (f.open(QIODevice::ReadOnly)) {
        QByteArray ar = f.read(8).toHex();
        QString s(ar);



        f.close();
    }

    return false;
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
