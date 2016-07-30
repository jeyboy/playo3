#include "extensions.h"

using namespace Core;

QString Extensions::cue_ext = QStringLiteral("cue");
QString Extensions::dot_cue_ext = QStringLiteral(".") % Extensions::cue_ext;

Extensions::Extensions() {
    ext = new DataStore(QStringLiteral("extensions.json"));

    if (ext -> state) {
        activeFilter = ext -> read(JSON_ACTIVE_KEY).toString(DEFAULT_ACTIVE);
        QJsonObject obj = ext -> read(JSON_KEY).toObject();

        foreach (QString key, obj.keys())
            filters.insert(key, obj.value(key).toVariant().value<QStringList>());

    } else {
        QStringList commonfiltersList;
        commonfiltersList << QStringLiteral("*");
        filters.insert(DEFAULT_ACTIVE, commonfiltersList);
        activeFilter = DEFAULT_ACTIVE;

        initMusicExtensions();
    }

    initSignatures();
}

Extensions::~Extensions() {
    ext -> clear();
    ext -> write(JSON_ACTIVE_KEY, activeFilterName());

    QJsonObject filtersObj;
    for(QHash<QString, QStringList>::Iterator it = filters.begin(); it != filters.end(); it++)
        filtersObj.insert(it.key(), QJsonValue::fromVariant(it.value()));

    ext -> write(JSON_KEY, filtersObj);
    ext -> save();
    delete ext;
}

void Extensions::initSignatures() {
    ext_signatures.insert(QStringLiteral("6d6f6f76"), QStringLiteral("mov"));                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert(QStringLiteral("6d6f6f76"), 4);

    ext_signatures.insert(QStringLiteral("66726565"), QStringLiteral("mov"));                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert(QStringLiteral("66726565"), 4);

    ext_signatures.insert(QStringLiteral("6d646174"), QStringLiteral("mov"));                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert(QStringLiteral("6d646174"), 4);

    ext_signatures.insert(QStringLiteral("77696465"), QStringLiteral("mov"));                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert(QStringLiteral("77696465"), 4);

    ext_signatures.insert(QStringLiteral("706e6f74"), QStringLiteral("mov"));                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert(QStringLiteral("706e6f74"), 4);

    ext_signatures.insert(QStringLiteral("736b6970"), QStringLiteral("mov"));                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert(QStringLiteral("736b6970"), 4);

    ext_signatures.insert(QStringLiteral("0000001466747970"), QStringLiteral("3gp"));                       // 3GPP multimedia files
    ext_signatures.insert(QStringLiteral("0000002066747970"), QStringLiteral("3gp"));                       // 3GPP2 multimedia files
    ext_signatures.insert(QStringLiteral("0000001866747970"), QStringLiteral("3gp5"));                      // MPEG-4 video files
    ext_signatures.insert(QStringLiteral("464f524d00"), QStringLiteral("aiff"));                            // Audio Interchange File
    ext_signatures.insert(QStringLiteral("3026b2758e66cf11"), QStringLiteral("asf" /*"wma" "wmv"*/));       // Windows Media Audio|Video File
    ext_signatures.insert(QStringLiteral("a6d900aa0062ce6c"), QStringLiteral("asf" /*"wma" "wmv"*/));       // Windows Media Audio|Video File
    ext_signatures.insert(QStringLiteral("5343486c"), QStringLiteral("ast"));                               // Underground Audio
    ext_signatures.insert(QStringLiteral("52494646"), QStringLiteral("dat" /*"4xm" "avi" "wav"*/));         // Video CD MPEG movie
    ext_signatures.insert(QStringLiteral("574d4d50"), QStringLiteral("dat"));                               // Walkman MP3 file
    ext_signatures.insert(QStringLiteral("664c6143"), QStringLiteral("flac"));                              // Free Lossless Audio Codec file
    ext_signatures.insert(QStringLiteral("4d4143"), QStringLiteral("ape"));                                 // Monkeys Lossless Audio Codec file
    ext_signatures.insert(QStringLiteral("464c56"), QStringLiteral("flv"));                                 // Flash video file
    ext_signatures.insert(QStringLiteral("445644"), QStringLiteral("ifo"));                                 // DVD info file
    ext_signatures.insert(QStringLiteral("2e524543"), QStringLiteral("ivr"));                               // RealPlayer video file (V11+)
    ext_signatures.insert(QStringLiteral("00000020667479704d3441"), QStringLiteral("m4a"));                 // Apple audio and video files
    ext_signatures.insert(QStringLiteral("4d546864"), QStringLiteral("midi"));                              // MIDI sound file
    ext_signatures.insert(QStringLiteral("1a45dfa393428288"), QStringLiteral("mkv"));                       // Matroska stream file
    ext_signatures.insert(QStringLiteral("494433"), QStringLiteral("mp3"));                                 // MP3 audio file
    ext_signatures.insert(QStringLiteral("fffb"), QStringLiteral("mp3"));                                   // MP3 audio file
    ext_signatures.insert(QStringLiteral("000001ba"), QStringLiteral("mpg" /*"vob"*/));                     // DVD video file
    ext_signatures.insert(QStringLiteral("000001b3"), QStringLiteral("mpg"));                               // MPEG video file
    ext_signatures.insert(QStringLiteral("4f67675300020000"), QStringLiteral("ogg" /*"oga" "ogv" "ogx"*/)); // Ogg Vorbis Codec compressed file
    ext_signatures.insert(QStringLiteral("2e524d4600000012"), QStringLiteral("ra"));                        // RealAudio file
    ext_signatures.insert(QStringLiteral("2e7261fd00"), QStringLiteral("ra"));                              // RealAudio streaming media
    ext_signatures.insert(QStringLiteral("2e524d46"), QStringLiteral("rm" /*rmvb*/));                       // RealMedia streaming media

    ext_signatures.insert(QStringLiteral("646e732e"), QStringLiteral("au"));                                // Audacity audio file

    ext_signatures.insert(QStringLiteral("47494638"), QStringLiteral("gif"));                               // GIF file
    ext_signatures.insert(QStringLiteral("424d"), QStringLiteral("bmp" /*"dib"*/));                         // Bitmap image
    ext_signatures.insert(QStringLiteral("b168de3a"), QStringLiteral("dcx"));                               // PCX bitmap
    ext_signatures.insert(QStringLiteral("7801730d626260"), QStringLiteral("dmg"));                         // MacOS X image file
    ext_signatures.insert(QStringLiteral("ffd8ffe0"), QStringLiteral("jfif" /*"jpg" "jpe" "jpeg"*/));       // JPEG IMAGE
    ext_signatures.insert(QStringLiteral("0ced"), QStringLiteral("mp"));                                    // Monochrome Picture TIFF bitmap
    ext_signatures.insert(QStringLiteral("89504e470d0a1a0a"), QStringLiteral("png"));                       // PNG image
    ext_signatures.insert(QStringLiteral("492049"), QStringLiteral("tif"));                                 // TIFF file
    ext_signatures.insert(QStringLiteral("49492a00"), QStringLiteral("tif"));                               // TIFF file
    ext_signatures.insert(QStringLiteral("4d4d002a"), QStringLiteral("tif"));                               // TIFF file
    ext_signatures.insert(QStringLiteral("4d4d002b"), QStringLiteral("tif"));                               // TIFF file

    ext_signatures.insert(QStringLiteral("25504446"), QStringLiteral("pdf" /*"fdf"*/));                     // PDF file
    ext_signatures.insert(QStringLiteral("dba52d00"), QStringLiteral("doc"));                               // Word 2.0 file
    ext_signatures.insert(QStringLiteral("504b030414000600"), QStringLiteral("docx" /*"pptx" "xlsx"*/));    // MS Office 2007 documents
    ext_signatures.insert(QStringLiteral("d0cf11e0a1b11ae1"), QStringLiteral("ppt" /*"pps"*/));             // Microsoft Office document
    ext_signatures.insert(QStringLiteral("4d534346"), QStringLiteral("ppz"));                               // Powerpoint Packaged Presentation

    ext_signatures.insert(QStringLiteral("377a bc af 27 1c"), QStringLiteral("7z"));                        // 7-Zip compressed file
    ext_signatures.insert(QStringLiteral("4172 43 01"), QStringLiteral("arc"));                             // FreeArc compressed file
    ext_signatures.insert(QStringLiteral("1a02"), QStringLiteral("arc"));                                   // LH archive (old vers.|type 1)
    ext_signatures.insert(QStringLiteral("1a03"), QStringLiteral("arc"));                                   // LH archive (old vers.|type 2)
    ext_signatures.insert(QStringLiteral("1a04"), QStringLiteral("arc"));                                   // LH archive (old vers.|type 3)
    ext_signatures.insert(QStringLiteral("1a08"), QStringLiteral("arc"));                                   // LH archive (old vers.|type 4)
    ext_signatures.insert(QStringLiteral("1a09"), QStringLiteral("arc"));                                   // LH archive (old vers.|type 5)
    ext_signatures.insert(QStringLiteral("60ea"), QStringLiteral("arj"));                                   // ARJ Compressed archive file
    ext_signatures.insert(QStringLiteral("425a68"), QStringLiteral("bz2"));                                 // bzip2 compressed archive
    ext_signatures.insert(QStringLiteral("1f8b08"), QStringLiteral("gz"));                                  // GZIP archive file
    ext_signatures.insert(QStringLiteral("5f27a889"), QStringLiteral("jar"));                               // Jar archive
    ext_signatures.insert(QStringLiteral("4a4152435300"), QStringLiteral("jar"));                           // JARCS compressed archive
    ext_signatures.insert(QStringLiteral("526172211a07"), QStringLiteral("rar"));                           // RAR archive
    ext_signatures.insert(QStringLiteral("504b0506"), QStringLiteral("zip"));                               // PKZIP archive
    ext_signatures.insert(QStringLiteral("504b0708"), QStringLiteral("zip"));                               // PKZIP archive
    ext_signatures.insert(QStringLiteral("5a4f4f20"), QStringLiteral("zoo"));                               // ZOO compressed archive

    ext_signatures.insert(QStringLiteral("4d5a"), QStringLiteral("exe"));                                   // Executable
}

QString Extensions::folderName(QFileInfo & info) {
    QString name;
    if (info.isDir())
        name = info.filePath();
    else
        name = info.dir().path();
    return name.split('/').last();
}

bool Extensions::extractExtension(QString & fileName, QString & sExt, bool extract) {
    sExt = fileName.section('.', -1, -1);
    if (sExt != fileName && sExt.indexOf(' ') == -1) {
        if (extract)
            fileName = fileName.section('.', 0, -2);
        return true;
    } else {
        sExt.clear();
        return false;
    }
}

bool Extensions::restoreExtension(QString & file_path, QString & restoredExt) {
    QFile f(file_path);
    bool finded = false;

    if (f.open(QIODevice::ReadOnly)) {
        QByteArray arHex = f.read(11).toHex();
        QString sH(arHex);

        for(QHash<QString, QString>::Iterator it = ext_signatures.begin(); it != ext_signatures.end(); it++) {
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

bool Extensions::respondToExtension(const QString & ext) const {
    return activeFilterList().contains(QStringLiteral("*.") % ext) || activeFilterList().contains(QStringLiteral("*.*")) || activeFilterList().contains(QStringLiteral("*"));
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
    mList << QStringLiteral("*.aif");   // Audio Interchange File
    mList << QStringLiteral("*.aifc");  // Audio Interchange File
    mList << QStringLiteral("*.aiff");  // Audio Interchange File
    mList << QStringLiteral("*.asf");   // Windows Media formats
    mList << QStringLiteral("*.adt");   // Windows Media formats
    mList << QStringLiteral("*.adts");  // Windows Media formats
    mList << QStringLiteral("*.cda");   // CD Audio track
    mList << QStringLiteral("*.flac");  // Audio files encoded by Flac - free lossless audio codec
    mList << QStringLiteral("*.fla");   // Audio files encoded by Flac - free lossless audio codec
    mList << QStringLiteral("*.mp1");   // Compressed audio and music file
    mList << QStringLiteral("*.mp2");   // Compressed audio and music file
    mList << QStringLiteral("*.mpa");   // Compressed audio and music file
    mList << QStringLiteral("*.mp3");   // Compressed audio and music file, mp3 songs or ringtones
    mList << QStringLiteral("*.amr");   // Adaptive Multi-Rate compressed audio file
    mList << QStringLiteral("*.wv");    // WavPack lossless compressed audio file
    mList << QStringLiteral("*.ram");   // Real Audio metafile
    mList << QStringLiteral("*.m4a");   // MPEG-4 compressed audio file
    mList << QStringLiteral("*.wav");   // WAVe PCM Sound, standard Windows sound format
    mList << QStringLiteral("*.ad4");   // 4D Systems audio file
    mList << QStringLiteral("*.thd");   // Dolby TrueHD audio file
    mList << QStringLiteral("*.wma");   // Windows Media Audio
    mList << QStringLiteral("*.aac");   // Advanced audio coding file
    mList << QStringLiteral("*.m4r");   // Apple iPhone ringtone format
    mList << QStringLiteral("*.mogg");  // Multitrack digital audio format
    mList << QStringLiteral("*.spx");   // Speex encoded audio
    mList << QStringLiteral("*.ogg");   // Ogg Vorbis audio file
    mList << QStringLiteral("*.au");    // Unix audio sound file format
    mList << QStringLiteral("*.pcm");   // PCM audio file
    mList << QStringLiteral("*.m4p");   // Apple iTunes Music Store audio file
    mList << QStringLiteral("*.mka");   // Matroska audio file
    mList << QStringLiteral("*.snd");   // Sound file
    mList << QStringLiteral("*.swa");   // Adobe Shockwave music file
    mList << QStringLiteral("*.aob");   // DVD Audio Object file
    mList << QStringLiteral("*.rms");   // RealMedia Secure Media file format
    mList << QStringLiteral("*.m4b");   // MPEG-4 Audio Layer audio book file
    mList << QStringLiteral("*.acm");   // ACM compressed sound file

    filters.insert(MUSIC_PRESET, mList);
}
