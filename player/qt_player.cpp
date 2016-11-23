#include "qt_player.h"

bool QtPlayer::proceedErrorState() {
    updateState(UnknowState);

    // TODO: write me
    // player -> errorString()

//    int err_code = BASS_ErrorGetCode();
//    qCritical() << "proceedErrorState" << media_url.toString() << err_code;
//    switch(err_code) {
//        case BASS_OK: return false;
//        case BASS_ERROR_FILEFORM: { emit statusChanged(media_title, InvalidMedia); break; }
//        case BASS_ERROR_FILEOPEN: { emit statusChanged(media_title, NoMedia); break; }
//        // BASS_ERROR_TIMEOUT
//        default:
//            qDebug() << "UNKNOWN ERROR: " << BASS_ErrorGetCode();
//            emit statusChanged(media_title, StalledMedia);
//    }

    return true;
}

void QtPlayer::mediaStatusChanged(QMediaPlayer::MediaStatus status) {
    switch(status) {
        case QMediaPlayer::NoMedia:         { emit statusChanged(title(), NoMedia); break;}
        case QMediaPlayer::LoadingMedia:    { emit statusChanged(title(), LoadingMedia); break; }
        case QMediaPlayer::LoadedMedia:     { emit statusChanged(title(), LoadedMedia); break; }
        case QMediaPlayer::StalledMedia:    { emit statusChanged(title(), StalledMedia); break; }
        case QMediaPlayer::EndOfMedia:      { emit statusChanged(title(), EndPlaingMedia); break; }
        case QMediaPlayer::InvalidMedia:    { emit statusChanged(title(), InvalidMedia); break; }
        case QMediaPlayer::BufferingMedia:  return; //not used
        case QMediaPlayer::BufferedMedia:   return; //not used
        default: emit statusChanged(title(), UnknownMediaStatus);
    }
}

bool QtPlayer::playProcessing(const bool & paused) {
    emit statusChanged(title(), InitMedia);

    //FIXME: need to use QNetworkRequest
    player -> setMedia(QMediaContent(media_url));

    player -> play();

    if (player -> state() == QMediaPlayer::PlayingState)
        playPostprocessing();

    emit videoOutputRequired(hasVideo());

    if (paused) player -> pause();

    return player -> error() == QMediaPlayer::NoError;
}
bool QtPlayer::resumeProcessing() { player -> play(); return true; }
bool QtPlayer::pauseProcessing() { player -> pause(); return true; }
bool QtPlayer::stopProcessing() { player -> stop(); return true; }

bool QtPlayer::newTempoProcessing(const int & new_tempo) {
    float tempo = new_tempo;
    if (tempo != 0)
        tempo = (new_tempo / (float)tempoMax()) * (tempo < 0 ? 0.5 : 2);

    player -> setPlaybackRate(1 + tempo);
    return true;
}
bool QtPlayer::newPosProcessing(const qint64 & new_pos) {
    player -> setPosition(new_pos);
    return true;
}
bool QtPlayer::newVolumeProcessing(const int & new_vol) {
    player -> setVolume(new_vol == 0 ? 0 : new_vol / QT_VOLUME_MULTIPLIER);
    return true;
}

bool QtPlayer::fileInfo(const QUrl & uri, IMediaInfo * /*info*/) {
//    QString Title;
//    QString SubTitle;
//    QString Author;
//    QString Comment;
//    QString Description;
//    QString Category;
//    QString Genre;
//    QString Year;
//    QString Date;
//    QString UserRating;
//    QString Keywords;
//    QString Language;
//    QString Publisher;
//    QString Copyright;
//    QString ParentalRating;
//    QString RatingOrganization;

//    // Media
//    QString Size;
//    QString MediaType;
//    QString Duration;

//    // Audio
//    QString AudioBitRate;
//    QString AudioCodec;
//    QString AverageLevel;
//    QString ChannelCount;
//    QString PeakValue;
//    QString SampleRate;

//    // Music
//    QString AlbumTitle;
//    QString AlbumArtist;
//    QString ContributingArtist;
//    QString Composer;
//    QString Conductor;
//    QString Lyrics;
//    QString Mood;
//    QString TrackNumber;
//    QString TrackCount;

//    QString CoverArtUrlSmall;
//    QString CoverArtUrlLarge;

//    // Image/Video
//    QString Resolution;
//    QString PixelAspectRatio;

//    // Video
//    QString VideoFrameRate;
//    QString VideoBitRate;
//    QString VideoCodec;

//    QString PosterUrl;

//    // Movie
//    QString ChapterNumber;
//    QString Director;
//    QString LeadPerformer;
//    QString Writer;

//    // Photos
//    QString CameraManufacturer;
//    QString CameraModel;
//    QString Event;
//    QString Subject;
//    QString Orientation;
//    QString ExposureTime;
//    QString FNumber;
//    QString ExposureProgram;
//    QString ISOSpeedRatings;
//    QString ExposureBiasValue;
//    QString DateTimeOriginal;
//    QString DateTimeDigitized;
//    QString SubjectDistance;
//    QString MeteringMode;
//    QString LightSource;
//    QString Flash;
//    QString FocalLength;
//    QString ExposureMode;
//    QString WhiteBalance;
//    QString DigitalZoomRatio;
//    QString FocalLengthIn35mmFilm;
//    QString SceneCaptureType;
//    QString GainControl;
//    QString Contrast;
//    QString Saturation;
//    QString Sharpness;
//    QString DeviceSettingDescription;

//    // Location
//    QString GPSLatitude;
//    QString GPSLongitude;
//    QString GPSAltitude;
//    QString GPSTimeStamp;
//    QString GPSSatellites;
//    QString GPSStatus;
//    QString GPSDOP;
//    QString GPSSpeed;
//    QString GPSTrack;
//    QString GPSTrackRef;
//    QString GPSImgDirection;
//    QString GPSImgDirectionRef;
//    QString GPSMapDatum;
//    QString GPSProcessingMethod;
//    QString GPSAreaInformation;

//    QString PosterImage;
//    QString CoverArtImage;
//    QString ThumbnailImage;


    if (player -> currentMedia().canonicalUrl() == uri) {
        if (player -> isMetaDataAvailable()) {
            QStringList keys = player -> availableMetaData();
            qDebug() << "KEYS LIST" << keys;

            for(QStringList::Iterator key = keys.begin(); key != keys.end(); key++) {
                QVariant val = player -> metaData(*key);
                int y = 0;
            }

            return true;
        }

        return false;
    }

    return false;
}

qint64 QtPlayer::calcFileSize() { return player -> mediaStream() -> size(); }

//    bool registerEQ();
//    bool unregisterEQ();
//    bool processEqSetGain(const int & band, const float & gain);

//    bool calcSpectrum(QVector<float> & result);
//    bool calcSpectrum(QList<QVector<float> > & result);


QtPlayer::QtPlayer(QWidget * parent) : IPlayer(parent), player(new QMediaPlayer(parent)) {
    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(mediaStatusChanged(QMediaPlayer::MediaStatus)));
}

QtPlayer::~QtPlayer() { player -> deleteLater(); }

qint64 QtPlayer::position() const { return player -> position(); }
