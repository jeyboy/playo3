#include "mediainfo.h"
#include "player/player_index.h"

using namespace Core::Media;

MediaInfo::MediaInfo(const QUrl & uri, const QVariant & extension, bool onlyTags) : IMediaInfo(!uri.isLocalFile()), fileName(0) {
    if (extension.isValid())
        ext = extension.toString();

    if (!remote) {
        QString file_path = uri.toLocalFile();

        // taglib not worked with files without extensions :(
        if (!extension.isValid())
            Extensions::obj().restoreExtension(file_path, ext);

//        error = !TagLib::FileRef::defaultFileExtensions.contains(TagLib::String(ext.toLower().toStdWString()));

        #ifdef Q_OS_WIN
            fileName = new TagLib::FileName(file_path.toStdWString().data());
        #else
            fileName = new TagLib::FileName(QFile::encodeName(file_path).toStdString().data());
        #endif
        TagLib::FileRef f(*fileName, !onlyTags, onlyTags ? TagLib::AudioProperties::Fast : TagLib::AudioProperties::Accurate);

        if (!f.isNull()) {
            readed = true;
            size = f.file() -> length();
            artist = QString::fromStdWString(f.tag() -> artist().toWString());
            title = QString::fromStdWString(f.tag() -> title().toWString());
            album = QString::fromStdWString(f.tag() -> album().toWString());
            setGenre(QString::fromStdWString(f.tag() -> genre().toWString()));
            year = f.tag() -> year();
            track = f.tag() -> track();

            if (!onlyTags)
                readInfo(f);
        } else {
            QFile f(file_path);
            error = f.error() != QFile::NoError;
            size = f.size();
            f.close();
        }
    }
    else if (!onlyTags) {
        readed = true;
        error = !PlayerFactory::obj().currPlayer() -> fileInfo(uri, this); // this method only initiating tech info
    }
}

void MediaInfo::initInfo() {
    TagLib::FileRef f(*fileName, true, TagLib::AudioProperties::Accurate);
    readInfo(f);
    takeArt(f);
}

void MediaInfo::readInfo(const TagLib::FileRef & f) {
    channels = f.audioProperties() -> channels();
    duration = f.audioProperties() -> length();
    sampleRate = f.audioProperties() -> sampleRate();

    bitrate = f.audioProperties() -> bitrate();
    if (bitrate == 0 && duration > 0)
        bitrate = calcAverageBitrate();
}

//TODO: finish me
void MediaInfo::takeArt(const TagLib::FileRef & f) {
    //#include <mpegfile.h>
    //#include <attachedpictureframe.h>
    //#include <id3v2tag.h>
    //#include <mp4file.h>
    //#include <mp4tag.h>
    //#include <mp4coverart.h>
    //#include <vorbisfile.h>
    //#include <xiphcomment.h>
    //#include <flacpicture.h>

    //#include <iostream>

    //extern "C"
    //{
    //#include "base64.h"
    //}

    //class ImageFile : public TagLib::File
    //{
    //public:
    //    ImageFile(const char *file) : TagLib::File(file)
    //    {

    //    }

    //    TagLib::ByteVector data()
    //    {
    //        return readBlock(length());
    //    }


    //private:
    //    virtual TagLib::Tag *tag() const { return 0; }
    //    virtual TagLib::AudioProperties *audioProperties() const { return 0; }
    //    virtual bool save() { return false; }
    //};

    //int main(int argc, char *argv[])
    //{
    //    if (argc != 3)
    //    {
    //        std::cout << "Usage: setcover <mp3|m4a|ogg> cover.jpg" << std::endl;
    //        return 1;
    //    }

    //    TagLib::String fileName = argv[1];
    //    TagLib::String fileType = fileName.substr(fileName.size() - 3).upper();

    //    ImageFile imageFile(argv[2]);
    //    TagLib::ByteVector imageData = imageFile.data();

    //    if (fileType == "M4A")
    //    {
    //      // read the image file
    //      TagLib::MP4::CoverArt coverArt((TagLib::MP4::CoverArt::Format) 0x0D, imageData);

    //      // read the mp4 file
    //      TagLib::MP4::File audioFile(argv[1]);

    //      // get the tag ptr
    //      TagLib::MP4::Tag *tag = audioFile.tag();

    //      // get the items map
    //      TagLib::MP4::ItemListMap itemsListMap = tag->itemListMap();

    //      // create cover art list
    //      TagLib::MP4::CoverArtList coverArtList;

    //      // append instance
    //      coverArtList.append(coverArt);

    //      // convert to item
    //      TagLib::MP4::Item coverItem(coverArtList);

    //      // add item to map
    //      itemsListMap.insert("covr", coverItem);

    //      tag->save();
    //    }
    //    else if (fileType == "MP3")
    //    {
    //      TagLib::MPEG::File audioFile(argv[1]);

    //      TagLib::ID3v2::Tag *tag = audioFile.ID3v2Tag(true);
    //      TagLib::ID3v2::AttachedPictureFrame *frame = new TagLib::ID3v2::AttachedPictureFrame;

    //      frame->setMimeType("image/jpeg");
    //      frame->setPicture(imageData);

    //      tag->addFrame(frame);
    //      audioFile.save();
    //    }
    //    else if (fileType == "OGG")
    //    {
    //      TagLib::Ogg::Vorbis::File audioFile(argv[1]);
    //      TagLib::Ogg::XiphComment *tag = audioFile.tag();

    //      /*
    //      PROPOSED http://wiki.xiph.org/VorbisComment#METADATA_BLOCK_PICTURE
    //      */
    //      TagLib::FLAC::Picture* picture = new TagLib::FLAC::Picture();
    //      picture->setData(imageData);
    //      picture->setType((TagLib::FLAC::Picture::Type)  0x03); // FrontCover
    //      picture->setMimeType("image/jpeg");
    //      picture->setDescription("Front Cover");

    //      TagLib::ByteVector block = picture->render();
    //      tag->addField("METADATA_BLOCK_PICTURE", b64_encode(block.data(), block.size()), true);


    //      /*
    //      UNOFFICIAL DEPRECATED http://wiki.xiph.org/VorbisComment#Unofficial_COVERART_field_.28deprecated.29
    //      */
    //      block = imageData;

    //      tag->addField("COVERART",  b64_encode(block.data(), block.size()), true);

    //      audioFile.save();
    //    }
    //    else
    //    {
    //       std::cout << fileType << " is unsupported." << std::endl;
    //    }
    //}



//    static Image getImage(const ID3v2::Tag *tag)
//    {
//        ID3v2::FrameList frames = tag->frameList("APIC");

//        if(frames.isEmpty())
//        {
//            return Image();
//        }

//        ID3v2::AttachedPictureFrame *frame =
//            static_cast<ID3v2::AttachedPictureFrame *>(frames.front());

//        return Image(frame->mimeType(), frame->picture());
//    }




//    static const char *IdPicture = “APIC” ;
//    TagLib::MPEG::File mpegFile(“/home/rajeev/Downloads/TamilBeat.Com – Dole Dole Than.mp3”);
//    TagLib::ID3v2::Tag *id3v2tag = mpegFile.ID3v2Tag();
//    TagLib::ID3v2::FrameList Frame ;
//    TagLib::ID3v2::AttachedPictureFrame *PicFrame ;
//    void *RetImage = NULL, *SrcImage ;
//    unsigned long Size ;

//    FILE *jpegFile;
//    jpegFile = fopen(“FromId3.jpg”,”wb”);

//    if ( id3v2tag )
//    {
//      // picture frame
//      Frame = id3v2tag->frameListMap()[IdPicture] ;
//      if (!Frame.isEmpty() )
//      {
//        for(TagLib::ID3v2::FrameList::ConstIterator it = Frame.begin(); it != Frame.end(); ++it)
//        {
//          PicFrame = (TagLib::ID3v2::AttachedPictureFrame *)(*it) ;
//          //  if ( PicFrame->type() ==
//          //TagLib::ID3v2::AttachedPictureFrame::FrontCover)
//          {
//            // extract image (in it’s compressed form)
//            Size = PicFrame->picture().size() ;
//            SrcImage = malloc ( Size ) ;
//            if ( SrcImage )
//            {
//              memcpy ( SrcImage, PicFrame->picture().data(), Size ) ;
//              fwrite(SrcImage,Size,1, jpegFile);
//              fclose(jpegFile);
//              free( SrcImage ) ;
//            }

//          }
//        }
//      }
//    }
//    else
//    {
//       cout<<“id3v2 not present”;
//    }
//    return(0);
//  }




//    void testPicture()
//    {
//      ScopedFileCopy copy("empty", ".ogg");
//      string newname = copy.fileName();

//      {
//        Vorbis::File f(newname.c_str());
//        FLAC::Picture *newpic = new FLAC::Picture();
//        newpic->setType(FLAC::Picture::BackCover);
//        newpic->setWidth(5);
//        newpic->setHeight(6);
//        newpic->setColorDepth(16);
//        newpic->setNumColors(7);
//        newpic->setMimeType("image/jpeg");
//        newpic->setDescription("new image");
//        newpic->setData("JPEG data");
//        f.tag()->addPicture(newpic);
//        f.save();
//      }
//      {
//        Vorbis::File f(newname.c_str());
//        List<FLAC::Picture *> lst = f.tag()->pictureList();
//        CPPUNIT_ASSERT_EQUAL((unsigned int)1, lst.size());
//        CPPUNIT_ASSERT_EQUAL((int)5, lst[0]->width());
//        CPPUNIT_ASSERT_EQUAL((int)6, lst[0]->height());
//        CPPUNIT_ASSERT_EQUAL((int)16, lst[0]->colorDepth());
//        CPPUNIT_ASSERT_EQUAL((int)7, lst[0]->numColors());
//        CPPUNIT_ASSERT_EQUAL(String("image/jpeg"), lst[0]->mimeType());
//        CPPUNIT_ASSERT_EQUAL(String("new image"), lst[0]->description());
//        CPPUNIT_ASSERT_EQUAL(ByteVector("JPEG data"), lst[0]->data());
//      }
//  }
}
