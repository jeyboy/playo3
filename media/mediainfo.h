#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include "taglib.h"
#include "tag.h"
#include "fileref.h"

#include "media/player.h"

#include <QString>

class Player;

class MediaInfo {
public:
    MediaInfo(QUrl uri, bool onlyTags = true);
    void initInfo();

    inline bool initiated() const { return readed; }

    inline QString getArtist() const { return artist; }
    inline QString getTitle() const { return title; }
    inline QString getAlbum() const { return album; }
    inline QString getGenre() const { return genre; }

    inline qint64 getSize() const { return size; }
    inline int getYear() const { return year; }
    inline int getTrack() const { return track; }
    inline int getChannels() const { return channels; }
    inline int getBitrate() const { return bitrate; }
    inline int getDuration() const { return duration; }
    inline int getSampleRate() const { return sampleRate; }
private:
    friend class Player;

    void readInfo(TagLib::FileRef f);

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
};

#endif // MEDIAINFO_H
