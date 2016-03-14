#ifndef IMEDIAINFO
#define IMEDIAINFO

#include <qstring.h>

#include "modules/core/media/genres/music_genres.h"

using namespace Core::Media;

class IMediaInfo {
protected:
    QString file_path;
    QString ext;
    QString artist;
    QString title;
    QString album;
    QString genre;

    int year;
    int track;

    int channels;
    int bitrate;
    qint64 duration;
    int sampleRate;
    qint64 size;

    bool error;
    bool readed;
    bool remote;

    inline int calcAverageBitrate() { return (size / (125 * duration) + 0.5);  /*average bitrate (Kbps)*/ }
public:
    IMediaInfo(bool is_remote = false) : year(-1), track(-1), channels(-1), bitrate(-1), duration(0),
        sampleRate(-1), size(0), error(false), readed(false), remote(is_remote) {}

    inline bool hasError() const { return error; }
    inline bool isReaded() const { return readed; }
    inline bool isRemote() const { return remote; }

    inline QString getArtist() const { return artist; }
    inline QString getTitle() const { return title; }
    inline QString getAlbum() const { return album; }
    inline int getGenre() const { return -1; /*genre*/; } //
    inline QString getExtension() const { return ext; }
    inline QString getPath() const { return file_path; }

    inline void setArtist(QString newArtist) { artist = newArtist; }
    inline void setTitle(QString newTitle) { title = newTitle; }
    inline void setAlbum(QString newAlbum) { album = newAlbum; }
    inline void setGenre(QString newGenre) { genre = newGenre; }
    inline void setGenre(int newGenre) { genre = MusicGenres::obj().toString(newGenre); } // need to update on global list init

    inline qint64 getSize() const { return size; }
    inline int getYear() const { return year; }
    inline int getTrack() const { return track; }
    inline int getChannels() const { return channels; }
    inline int getBitrate() const { return bitrate; }
    inline qint64 getDuration() const { return duration * 1000; } // convert to millis
    inline int getSampleRate() const { return sampleRate; }

    inline void setSize(qint64 newSize) { size = newSize; }
    inline void setYear(int newYear) { year = newYear; }
    inline void setTrack(int newTrack) { track = newTrack; }
    inline void setChannels(int newChannels) { channels = newChannels; }
    inline void setBitrate(int newBitrate) { bitrate = newBitrate; }
    inline void setDuration(int newDuration) { duration = newDuration; }
    inline void setSampleRate(int newSampleRate) { sampleRate = newSampleRate; }
};

#endif // IMEDIAINFO
