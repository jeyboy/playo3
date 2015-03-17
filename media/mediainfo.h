#ifndef MEDIAINFO_H
#define MEDIAINFO_H

#include "taglib.h"
#include "tag.h"
#include "fileref.h"

#include <QString>

class MediaInfo {
public:
    MediaInfo(QString filepath, bool onlyTags = true);
    void initInfo();

    bool initiated() const;

    QString getArtist() const;
    QString getTitle() const;
    QString getAlbum() const;
    QString getGenre() const;

    long getSize() const;
    int getYear() const;
    int getTrack() const;
    int getChannels() const;
    int getBitrate() const;
    int getDuration() const;
    int getSampleRate() const;
private:
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
    long size;

    bool readed;
};

#endif // MEDIAINFO_H
