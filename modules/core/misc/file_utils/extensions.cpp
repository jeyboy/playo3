#include "extensions.h"

using namespace Core;

QString Extensions::cue_ext = QLatin1String("cue");
QString Extensions::dot_cue_ext = QLatin1String(".") % Extensions::cue_ext;

Extensions::Extensions() {
    ext = new DataStore(QLatin1String("extensions.json"));

    if (ext -> state) {
        activeFilter = ext -> read(JSON_ACTIVE_KEY).toString(DEFAULT_ACTIVE);
        QJsonObject obj = ext -> read(JSON_KEY).toObject();

        foreach (QString key, obj.keys())
            filters.insert(key, obj.value(key).toVariant().value<QStringList>());

    } else {
        QStringList commonfiltersList;
        commonfiltersList << QLatin1String("*");
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
    ext_signatures.insert(QLatin1String("6d6f6f76"), QLatin1String("mov"));                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert(QLatin1String("6d6f6f76"), 4);

    ext_signatures.insert(QLatin1String("66726565"), QLatin1String("mov"));                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert(QLatin1String("66726565"), 4);

    ext_signatures.insert(QLatin1String("6d646174"), QLatin1String("mov"));                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert(QLatin1String("6d646174"), 4);

    ext_signatures.insert(QLatin1String("77696465"), QLatin1String("mov"));                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert(QLatin1String("77696465"), 4);

    ext_signatures.insert(QLatin1String("706e6f74"), QLatin1String("mov"));                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert(QLatin1String("706e6f74"), 4);

    ext_signatures.insert(QLatin1String("736b6970"), QLatin1String("mov"));                               // QuickTime movie // offset: 4 bytes
    ext_signature_offset.insert(QLatin1String("736b6970"), 4);

    ext_signatures.insert(QLatin1String("0000001466747970"), QLatin1String("3gp"));                       // 3GPP multimedia files
    ext_signatures.insert(QLatin1String("0000002066747970"), QLatin1String("3gp"));                       // 3GPP2 multimedia files
    ext_signatures.insert(QLatin1String("0000001866747970"), QLatin1String("3gp5"));                      // MPEG-4 video files
    ext_signatures.insert(QLatin1String("464f524d00"), QLatin1String("aiff"));                            // Audio Interchange File
    ext_signatures.insert(QLatin1String("3026b2758e66cf11"), QLatin1String("asf" /*"wma" "wmv"*/));       // Windows Media Audio|Video File
    ext_signatures.insert(QLatin1String("a6d900aa0062ce6c"), QLatin1String("asf" /*"wma" "wmv"*/));       // Windows Media Audio|Video File
    ext_signatures.insert(QLatin1String("5343486c"), QLatin1String("ast"));                               // Underground Audio
    ext_signatures.insert(QLatin1String("52494646"), QLatin1String("dat" /*"4xm" "avi" "wav"*/));         // Video CD MPEG movie
    ext_signatures.insert(QLatin1String("574d4d50"), QLatin1String("dat"));                               // Walkman MP3 file
    ext_signatures.insert(QLatin1String("664c6143"), QLatin1String("flac"));                              // Free Lossless Audio Codec file
    ext_signatures.insert(QLatin1String("4d4143"), QLatin1String("ape"));                                 // Monkeys Lossless Audio Codec file
    ext_signatures.insert(QLatin1String("464c56"), QLatin1String("flv"));                                 // Flash video file
    ext_signatures.insert(QLatin1String("445644"), QLatin1String("ifo"));                                 // DVD info file
    ext_signatures.insert(QLatin1String("2e524543"), QLatin1String("ivr"));                               // RealPlayer video file (V11+)
    ext_signatures.insert(QLatin1String("00000020667479704d3441"), QLatin1String("m4a"));                 // Apple audio and video files
    ext_signatures.insert(QLatin1String("4d546864"), QLatin1String("midi"));                              // MIDI sound file
    ext_signatures.insert(QLatin1String("1a45dfa393428288"), QLatin1String("mkv"));                       // Matroska stream file
    ext_signatures.insert(QLatin1String("494433"), QLatin1String("mp3"));                                 // MP3 audio file
    ext_signatures.insert(QLatin1String("fffb"), QLatin1String("mp3"));                                   // MP3 audio file
    ext_signatures.insert(QLatin1String("000001ba"), QLatin1String("mpg" /*"vob"*/));                     // DVD video file
    ext_signatures.insert(QLatin1String("000001b3"), QLatin1String("mpg"));                               // MPEG video file
    ext_signatures.insert(QLatin1String("4f67675300020000"), QLatin1String("ogg" /*"oga" "ogv" "ogx"*/)); // Ogg Vorbis Codec compressed file
    ext_signatures.insert(QLatin1String("2e524d4600000012"), QLatin1String("ra"));                        // RealAudio file
    ext_signatures.insert(QLatin1String("2e7261fd00"), QLatin1String("ra"));                              // RealAudio streaming media
    ext_signatures.insert(QLatin1String("2e524d46"), QLatin1String("rm" /*rmvb*/));                       // RealMedia streaming media

    ext_signatures.insert(QLatin1String("646e732e"), QLatin1String("au"));                                // Audacity audio file

    ext_signatures.insert(QLatin1String("47494638"), QLatin1String("gif"));                               // GIF file
    ext_signatures.insert(QLatin1String("424d"), QLatin1String("bmp" /*"dib"*/));                         // Bitmap image
    ext_signatures.insert(QLatin1String("b168de3a"), QLatin1String("dcx"));                               // PCX bitmap
    ext_signatures.insert(QLatin1String("7801730d626260"), QLatin1String("dmg"));                         // MacOS X image file
    ext_signatures.insert(QLatin1String("ffd8ffe0"), QLatin1String("jfif" /*"jpg" "jpe" "jpeg"*/));       // JPEG IMAGE
    ext_signatures.insert(QLatin1String("0ced"), QLatin1String("mp"));                                    // Monochrome Picture TIFF bitmap
    ext_signatures.insert(QLatin1String("89504e470d0a1a0a"), QLatin1String("png"));                       // PNG image
    ext_signatures.insert(QLatin1String("492049"), QLatin1String("tif"));                                 // TIFF file
    ext_signatures.insert(QLatin1String("49492a00"), QLatin1String("tif"));                               // TIFF file
    ext_signatures.insert(QLatin1String("4d4d002a"), QLatin1String("tif"));                               // TIFF file
    ext_signatures.insert(QLatin1String("4d4d002b"), QLatin1String("tif"));                               // TIFF file

    ext_signatures.insert(QLatin1String("25504446"), QLatin1String("pdf" /*"fdf"*/));                     // PDF file
    ext_signatures.insert(QLatin1String("dba52d00"), QLatin1String("doc"));                               // Word 2.0 file
    ext_signatures.insert(QLatin1String("504b030414000600"), QLatin1String("docx" /*"pptx" "xlsx"*/));    // MS Office 2007 documents
    ext_signatures.insert(QLatin1String("d0cf11e0a1b11ae1"), QLatin1String("ppt" /*"pps"*/));             // Microsoft Office document
    ext_signatures.insert(QLatin1String("4d534346"), QLatin1String("ppz"));                               // Powerpoint Packaged Presentation

    ext_signatures.insert(QLatin1String("377a bc af 27 1c"), QLatin1String("7z"));                        // 7-Zip compressed file
    ext_signatures.insert(QLatin1String("4172 43 01"), QLatin1String("arc"));                             // FreeArc compressed file
    ext_signatures.insert(QLatin1String("1a02"), QLatin1String("arc"));                                   // LH archive (old vers.|type 1)
    ext_signatures.insert(QLatin1String("1a03"), QLatin1String("arc"));                                   // LH archive (old vers.|type 2)
    ext_signatures.insert(QLatin1String("1a04"), QLatin1String("arc"));                                   // LH archive (old vers.|type 3)
    ext_signatures.insert(QLatin1String("1a08"), QLatin1String("arc"));                                   // LH archive (old vers.|type 4)
    ext_signatures.insert(QLatin1String("1a09"), QLatin1String("arc"));                                   // LH archive (old vers.|type 5)
    ext_signatures.insert(QLatin1String("60ea"), QLatin1String("arj"));                                   // ARJ Compressed archive file
    ext_signatures.insert(QLatin1String("425a68"), QLatin1String("bz2"));                                 // bzip2 compressed archive
    ext_signatures.insert(QLatin1String("1f8b08"), QLatin1String("gz"));                                  // GZIP archive file
    ext_signatures.insert(QLatin1String("5f27a889"), QLatin1String("jar"));                               // Jar archive
    ext_signatures.insert(QLatin1String("4a4152435300"), QLatin1String("jar"));                           // JARCS compressed archive
    ext_signatures.insert(QLatin1String("526172211a07"), QLatin1String("rar"));                           // RAR archive
    ext_signatures.insert(QLatin1String("504b0506"), QLatin1String("zip"));                               // PKZIP archive
    ext_signatures.insert(QLatin1String("504b0708"), QLatin1String("zip"));                               // PKZIP archive
    ext_signatures.insert(QLatin1String("5a4f4f20"), QLatin1String("zoo"));                               // ZOO compressed archive

    ext_signatures.insert(QLatin1String("4d5a"), QLatin1String("exe"));                                   // Executable
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
    return activeFilterList().contains(QLatin1String("*.") % ext) || activeFilterList().contains(QLatin1String("*.*")) || activeFilterList().contains(QLatin1String("*"));
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
    mList << QLatin1String("*.aif");   // Audio Interchange File
    mList << QLatin1String("*.aifc");  // Audio Interchange File
    mList << QLatin1String("*.aiff");  // Audio Interchange File
    mList << QLatin1String("*.asf");   // Windows Media formats
    mList << QLatin1String("*.adt");   // Windows Media formats
    mList << QLatin1String("*.adts");  // Windows Media formats
    mList << QLatin1String("*.cda");   // CD Audio track
    mList << QLatin1String("*.flac");  // Audio files encoded by Flac - free lossless audio codec
    mList << QLatin1String("*.fla");   // Audio files encoded by Flac - free lossless audio codec
    mList << QLatin1String("*.mp1");   // Compressed audio and music file
    mList << QLatin1String("*.mp2");   // Compressed audio and music file
    mList << QLatin1String("*.mpa");   // Compressed audio and music file
    mList << QLatin1String("*.mp3");   // Compressed audio and music file, mp3 songs or ringtones
    mList << QLatin1String("*.amr");   // Adaptive Multi-Rate compressed audio file
    mList << QLatin1String("*.wv");    // WavPack lossless compressed audio file
    mList << QLatin1String("*.ram");   // Real Audio metafile
    mList << QLatin1String("*.m4a");   // MPEG-4 compressed audio file
    mList << QLatin1String("*.wav");   // WAVe PCM Sound, standard Windows sound format
    mList << QLatin1String("*.ad4");   // 4D Systems audio file
    mList << QLatin1String("*.thd");   // Dolby TrueHD audio file
    mList << QLatin1String("*.wma");   // Windows Media Audio
    mList << QLatin1String("*.aac");   // Advanced audio coding file
    mList << QLatin1String("*.m4r");   // Apple iPhone ringtone format
    mList << QLatin1String("*.mogg");  // Multitrack digital audio format
    mList << QLatin1String("*.spx");   // Speex encoded audio
    mList << QLatin1String("*.ogg");   // Ogg Vorbis audio file
    mList << QLatin1String("*.au");    // Unix audio sound file format
    mList << QLatin1String("*.pcm");   // PCM audio file
    mList << QLatin1String("*.m4p");   // Apple iTunes Music Store audio file
    mList << QLatin1String("*.mka");   // Matroska audio file
    mList << QLatin1String("*.snd");   // Sound file
    mList << QLatin1String("*.swa");   // Adobe Shockwave music file
    mList << QLatin1String("*.aob");   // DVD Audio Object file
    mList << QLatin1String("*.rms");   // RealMedia Secure Media file format
    mList << QLatin1String("*.m4b");   // MPEG-4 Audio Layer audio book file
    mList << QLatin1String("*.acm");   // ACM compressed sound file

    filters.insert(MUSIC_PRESET, mList);
}
