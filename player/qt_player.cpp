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

bool QtPlayer::playProcessing(const bool & paused = false) {
    emit statusChanged(title(), InitMedia);
    player -> setMedia(QMediaContent(media_url));
    player -> play();

    if (player -> state() == QMediaPlayer::PlayingState)
        playPostprocessing();

    if (paused) player -> pause();
}
bool QtPlayer::resumeProcessing() { player -> play(); }
bool QtPlayer::pauseProcessing() { player -> pause(); }
bool QtPlayer::stopProcessing() { player -> stop(); }

bool QtPlayer::newTempoProcessing(const int & new_tempo) {
    float tempo = new_tempo;
    if (tempo != 0)
        tempo = (new_tempo / (float)tempoMax()) * (tempo < 0 ? 0.5 : 2);

    player -> setPlaybackRate(1 + tempo);
}
bool QtPlayer::newPosProcessing(const qint64 & new_pos) {
    player -> setPosition(new_pos);
}
bool QtPlayer::newVolumeProcessing(const int & new_vol) {
    player -> setVolume(new_volume == 0 ? 0 : new_volume / QT_VOLUME_MULTIPLIER);
}

bool QtPlayer::fileInfo(const QUrl & uri, IMediaInfo * /*info*/) {
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

qint64 QtPlayer::calcFileSize() { player -> mediaStream() -> size(); }

//    bool registerEQ();
//    bool unregisterEQ();
//    bool processEqSetGain(const int & band, const float & gain);

//    bool calcSpectrum(QVector<float> & result);
//    bool calcSpectrum(QList<QVector<float> > & result);


QtPlayer::QtPlayer(QWidget * parent) : player(new QMediaPlayer(parent)) {
    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)), this, SLOT(mediaStatusChanged(QMediaPlayer::MediaStatus)));
}

QtPlayer::~QtPlayer() { player -> deleteLater(); }

qint64 QtPlayer::position() const { return player -> position(); }
