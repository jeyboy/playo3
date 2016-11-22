#ifndef BASS_PLAYER
#define BASS_PLAYER

#include <qdir.h>

#include <QtConcurrent/qtconcurrentrun.h>
#include <qfuturewatcher.h>

#include "bass.h"
#include "bass_fx.h"
#include "bassmix.h"

#include "modules/core/media/interfaces/iplayer.h"


//char *BASS_ChannelGetTags(
//    DWORD handle,
//    DWORD tags
//);

//Parameters
//handle	The channel handle... a HMUSIC or HSTREAM.
//tags	The tags/headers wanted... one of the following.
//BASS_TAG_APE	APE (v1 or v2) tags. A pointer to a series of null-terminated UTF-8 strings is returned, the final string ending with a double null. Each string is in the form of "key=value", or "key=value1/value2/..." if there are multiple values.
//BASS_TAG_APE_BINARY
//+ tag number (0=first)	APE binary tag. A pointer to a TAG_APE_BINARY structure is returned.
//BASS_TAG_CA_CODEC	CoreAudio codec information. A pointer to a TAG_CA_CODEC structure is returned.
//BASS_TAG_HTTP	HTTP headers, only available when streaming from a HTTP server. A pointer to a series of null-terminated strings is returned, the final string ending with a double null.
//BASS_TAG_ICY	ICY (Shoutcast) tags. A pointer to a series of null-terminated strings is returned, the final string ending with a double null.
//BASS_TAG_ID3	ID3v1 tags. A pointer to a TAG_ID3 structure is returned.
//BASS_TAG_ID3V2	ID3v2 tags. A pointer to a variable length block is returned. ID3v2 tags are supported at both the start and end of the file, and in designated RIFF/AIFF chunks. See www.id3.org for details of the block's structure.
//BASS_TAG_LYRICS3	Lyrics3v2 tag. A single string is returned, containing the Lyrics3v2 information. See www.id3.org/Lyrics3v2 for details of its format.
//BASS_TAG_META	Shoutcast metadata. A single string is returned, containing the current stream title and url (usually omitted). The format of the string is: StreamTitle='xxx';StreamUrl='xxx';
//BASS_TAG_MF	Media Foundation metadata. A pointer to a series of null-terminated UTF-8 strings is returned, the final string ending with a double null.
//BASS_TAG_MP4	MP4/iTunes metadata. A pointer to a series of null-terminated UTF-8 strings is returned, the final string ending with a double null.
//BASS_TAG_MUSIC_AUTH	MOD music author. Only available in files created with the OpenMPT tracker.
//BASS_TAG_MUSIC_INST
//+ instrument number (0=first)	MOD instrument name. Only available with formats that have instruments, eg. IT and XM (and MO3).
//BASS_TAG_MUSIC_MESSAGE	MOD message text.
//BASS_TAG_MUSIC_NAME	MOD music title.
//BASS_TAG_MUSIC_ORDERS	MOD music order list. A pointer to a byte array is returned, with each byte being the pattern number played at that order position. Pattern number 254 is "+++" (skip order) and 255 is "---" (end song).
//BASS_TAG_MUSIC_SAMPLE
//+ sample number (0=first)	MOD sample name.
//BASS_TAG_OGG	OGG comments. A pointer to a series of null-terminated UTF-8 strings is returned, the final string ending with a double null.
//BASS_TAG_RIFF_BEXT	RIFF/BWF "bext" chunk tags. A pointer to a TAG_BEXT structure is returned.
//BASS_TAG_RIFF_CART	RIFF/BWF "cart" chunk tags. A pointer to a TAG_CART structure is returned.
//BASS_TAG_RIFF_DISP	RIFF "DISP" chunk text (CF_TEXT) tag. A single string is returned.
//BASS_TAG_RIFF_INFO	RIFF "INFO" chunk tags. A pointer to a series of null-terminated strings is returned, the final string ending with a double null. The tags are in the form of "XXXX=text", where "XXXX" is the chunk ID.
//BASS_TAG_VENDOR	OGG encoder. A single UTF-8 string is returned.
//BASS_TAG_WAVEFORMAT	WAVE "fmt " chunk contents. A pointer to a WAVEFORMATEX structure is returned. As well as WAVE files, this is also provided by Media Foundation codecs.
//BASS_TAG_WMA	WMA tags. A pointer to a series of null-terminated UTF-8 strings is returned, the final string ending with a double null.
//other tags may be supported by add-ons, see the documentation.

//Return value
//If successful, the requested tags are returned, else NULL is returned. Use BASS_ErrorGetCode to get the error code.

//Error codes
//BASS_ERROR_HANDLE	handle is not valid.
//BASS_ERROR_NOTAVAIL	The requested tags are not available.

//Remarks
//Some tags (eg. ID3v1) are located at the end of the file, so when streaming a file from the internet, the tags will not be available until the download is complete. A BASS_SYNC_DOWNLOAD sync can be set via BASS_ChannelSetSync, to be informed of when the download is complete. A BASS_SYNC_META sync can be used to be informed of new Shoutcast metadata, and a BASS_SYNC_OGG_CHANGE sync for when a new logical bitstream begins in a chained OGG stream, which generally brings new OGG tags.


void
    #ifdef Q_OS_WIN
        __stdcall
    #endif
        endTrackSync(HSYNC handle, DWORD channel, DWORD data, void * user);
void
    #ifdef Q_OS_WIN
        __stdcall
    #endif
        endTrackDownloading(HSYNC, DWORD, DWORD, void * user);

#ifdef Q_OS_WIN
    #define QSTRING_TO_STR(str) str.toStdWString().data()
#else
    #define QSTRING_TO_STR(str) str.toStdString().c_str()
#endif

#define BASS_POSITION_MULTIPLIER 1000.0

#define LOCAL_PLAY_ATTRS BASS_SAMPLE_FLOAT | BASS_ASYNCFILE | BASS_STREAM_DECODE
#define REMOTE_PLAY_ATTRS BASS_SAMPLE_FLOAT | BASS_STREAM_DECODE /*| BASS_STREAM_PRESCAN*/

// without tempo
//#define LOCAL_PLAY_ATTRS BASS_SAMPLE_FLOAT | BASS_ASYNCFILE
//#define REMOTE_PLAY_ATTRS BASS_SAMPLE_FLOAT /*| BASS_STREAM_PRESCAN*/

#define LOCAL_BPM_ATTRS BASS_SAMPLE_FLOAT | BASS_STREAM_DECODE | BASS_STREAM_PRESCAN //| BASS_SAMPLE_MONO
#define REMOTE_BPM_ATTRS BASS_SAMPLE_FLOAT | BASS_STREAM_DECODE //| BASS_SAMPLE_MONO

class BassPlayer : public IPlayer {
    Q_OBJECT

    HFX _fxEQ;
    HSYNC syncHandle, syncDownloadHandle;
    bool is_paused;
    qint64 chan;
    QFutureWatcher<QPair<QString, qint64> > * openChannelWatcher;
    QPair<QString, qint64> proc_channel;

    int default_device();

    bool proceedErrorState();
    QPair<QString, qint64> openChannel(const QUrl & url,  QPair<QString, qint64> & channel_params);
    void closeChannel();

    void playPreproccessing();
protected slots:
    void afterSourceOpening();

protected:
    bool isSupportTempoChanging() { return true; }

    void setSampleRateQuality() {
        // On Windows, sample rate conversion is handled by Windows or the output device/driver rather than BASS, so this setting has no effect on playback there.
        // 0 = linear interpolation, 1 = 8 point sinc interpolation, 2 = 16 point sinc interpolation, 3 = 32 point sinc interpolation
        BASS_ChannelSetAttribute(chan, BASS_ATTRIB_SRC, 3);
    }

    bool initDevice(const int & newDevice, const int & frequency = 44100);
    bool closeDevice(const int & device);
    void loadPlugins();

    inline qint64 open(const QString & path, const DWORD & flags) {
        return BASS_StreamCreateFile(false, QSTRING_TO_STR(path), 0, 0, flags);
    }
    inline qint64 openRemote(const QString & path, const DWORD & flags) {
        return BASS_StreamCreateURL(QSTRING_TO_STR(path), 0, flags, NULL, 0);
    }

    bool isSupportPanChanging() { return true; }

    bool playProcessing(const bool & paused = false);
    bool resumeProcessing();
    bool pauseProcessing();
    bool stopProcessing();

    void applyTempoToChannel();
    bool newTempoProcessing(const int & new_tempo);
    bool newPosProcessing(const qint64 & new_pos);
    bool newVolumeProcessing(const int & new_vol);
    bool newPanProcessing(const int & new_pan);
    float prebufferingLevelCalc();
    qint64 calcFileSize();

    bool registerEQ();
    bool unregisterEQ();
    bool processEqSetGain(const int & band, const float & gain);

    bool calcSpectrum(QVector<float> & result);
    bool calcSpectrum(QList<QVector<float> > & result);
public:
    explicit BassPlayer(QWidget * parent);
    ~BassPlayer();

    QHash<QString, QVariant> deviceList();
    QVariant currDevice();
    bool setDevice(const QVariant & device);

    qint64 position() const;

    bool fileInfo(const QUrl & uri, IMediaInfo * info);
    float bpmCalc(const QUrl & uri);

    inline bool setReadTimeOut(const float & sec_limit) { return BASS_SetConfig(BASS_CONFIG_NET_READTIMEOUT, qMax(1000, (int)(sec_limit * 1000))); }
    inline bool setOpenTimeOut(const float & sec_limit) { return BASS_SetConfig(BASS_CONFIG_NET_TIMEOUT, qMax(1000, (int)(sec_limit * 1000))); }
    inline bool setProxy(const QString & proxy_str = QString()) { return BASS_SetConfigPtr(BASS_CONFIG_NET_PROXY, proxy_str.isEmpty() ? NULL : QSTRING_TO_STR(proxy_str)); }

//    inline bool isTryingToOpenMedia() { return openChannelWatcher != 0 && openChannelWatcher -> isRunning(); }
//    inline void userAgent(const QString & user_agent = QString()) { BASS_SetConfigPtr(BASS_CONFIG_NET_AGENT, user_agent.isEmpty() ? NULL : QSTRING_TO_STR(user_agent)); }
};

#endif // BASS_PLAYER
