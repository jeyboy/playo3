#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include "taglib.h"
#include "tag.h"
#include "fileref.h"

#include "media/genres/music_genres.h"
#include "media/player.h"

#include <qurl.h>
#include <QString>

class MediaInfo {
public:
    MediaInfo(QUrl uri, bool onlyTags = true);
    void initInfo();

    inline bool isReaded() const { return readed; }
    inline bool isRemote() const { return remote; }

    inline QString getArtist() const { return artist; }
    inline QString getTitle() const { return title; }
    inline QString getAlbum() const { return album; }
    inline QString getGenre() const { return genre; }

    inline void setArtist(QString newArtist) { artist = newArtist; }
    inline void setTitle(QString newTitle) { title = newTitle; }
    inline void setAlbum(QString newAlbum) { album = newAlbum; }
    inline void setGenre(QString newGenre) { genre = newGenre; }

    inline qint64 getSize() const { return size; }
    inline int getYear() const { return year; }
    inline int getTrack() const { return track; }
    inline int getChannels() const { return channels; }
    inline int getBitrate() const { return bitrate; }
    inline int getDuration() const { return duration; }
    inline int getSampleRate() const { return sampleRate; }

    inline void setSize(qint64 newSize) { size = newSize; }
    inline void setYear(int newYear) { year = newYear; }
    inline void setTrack(int newTrack) { track = newTrack; }
    inline void setChannels(int newChannels) { channels = newChannels; }
    inline void setBitrate(int newBitrate) { bitrate = newBitrate; }
    inline void setDuration(int newDuration) { duration = newDuration; }
    inline void setSampleRate(int newSampleRate) { sampleRate = newSampleRate; }
private:
    void readInfo(TagLib::FileRef f);
    inline int calcAverageBitrate() { return (size / (125 * duration) + 0.5);  /*average bitrate (Kbps)*/ }

    std::string fileName;

    QString artist;
    QString title;
    QString album;
    QString genre;

    int year;
    int track;

    int channels;
    int bitrate;
    int duration;
    int sampleRate;
    qint64 size;

    bool readed;
    bool remote;
};

#endif // MEDIAINFO_H
