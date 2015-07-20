#ifndef UNICODE_DECODING
#define UNICODE_DECODING

#include <qiodevice.h>
#include <qdebug.h>

class UnicodeDecoding {
protected:
    inline bool isUnicodeNonCharacter(uint ucs4) {
        // Unicode has a couple of "non-characters" that one can use internally,
        // but are not allowed to be used for text interchange.
        //
        // Those are the last two entries each Unicode Plane (U+FFFE, U+FFFF,
        // U+1FFFE, U+1FFFF, etc.) as well as the entries between U+FDD0 and
        // U+FDEF (inclusive)

        return (ucs4 & 0xfffe) == 0xfffe || (ucs4 - 0xfdd0U) < 16;
    }

    inline void scanUtf8Char(QIODevice * io, QString & result, char & in) {
        int need;
        uint min_uc, uc;

        if ((in & 0xe0) == 0xc0) {
            uc = in & 0x1f;
            need = 1;
            min_uc = 0x80;
        } else if ((in & 0xf0) == 0xe0) {
            uc = in & 0x0f;
            need = 2;
            min_uc = 0x800;
        } else if ((in&0xf8) == 0xf0) {
            uc = in & 0x07;
            need = 3;
            min_uc = 0x10000;
        } else {
            qDebug() << "BLIA " << in;
            result.append(in);
            return;
        }

//        if (io -> bytesAvailable() < need) { result.append(ch); return;}

        for (int i = 0; i < need; ++i) {
            io -> getChar(&in);
            if ((in&0xc0) != 0x80) { qDebug() << "PIPEC" << in; return; }
            uc = (uc << 6) | (in & 0x3f);
        }

        if (isUnicodeNonCharacter(uc) || uc >= 0x110000 || (uc < min_uc) || (uc >= 0xd800 && uc <= 0xdfff)) {
            qDebug() << "UNEBABLE"; return;
        }

        if (QChar::requiresSurrogates(uc)) { result.append(QChar::highSurrogate(uc)); result.append(QChar::lowSurrogate(uc)); }
        else result.append(QChar(uc));
    }

    inline void scanUtf8Char(char * io, QString & result) {
        int need;
        uint min_uc, uc;

        if (((*io) & 0xe0) == 0xc0) {
            uc = (*io) & 0x1f;
            need = 1;
            min_uc = 0x80;
        } else if (((*io) & 0xf0) == 0xe0) {
            uc = (*io) & 0x0f;
            need = 2;
            min_uc = 0x800;
        } else if (((*io)&0xf8) == 0xf0) {
            uc = (*io) & 0x07;
            need = 3;
            min_uc = 0x10000;
        } else {
            qDebug() << "BLIA " << (*io);
            result.append((*io));
            return;
        }

//        if (io -> bytesAvailable() < need) { result.append(ch); return;}

        for (int i = 0; i < need; ++i, io++) {
            if (((*io)&0xc0) != 0x80) { qDebug() << "PIPEC" << (*io); return; }
            uc = (uc << 6) | ((*io) & 0x3f);
        }

        if (isUnicodeNonCharacter(uc) || uc >= 0x110000 || (uc < min_uc) || (uc >= 0xd800 && uc <= 0xdfff)) {
            qDebug() << "UNEBABLE"; return;
        }

        if (QChar::requiresSurrogates(uc)) { result.append(QChar::highSurrogate(uc)); result.append(QChar::lowSurrogate(uc)); }
        else result.append(QChar(uc));
    }
};

#endif // UNICODE_DECODING
