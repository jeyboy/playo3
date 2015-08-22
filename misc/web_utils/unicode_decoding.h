#ifndef UNICODE_DECODING
#define UNICODE_DECODING

#include <qiodevice.h>
#include <qdebug.h>

class UnicodeDecoding {
protected:
    enum CharsetType {
        unknown,
        utf8,
        cp1251
    };

    const int cp1251_table[64] = {
      0x0402, 0x0403, 0x201A, 0x0453, 0x201E, 0x2026, 0x2020, 0x2021,
      0x20AC, 0x2030, 0x0409, 0x2039, 0x040A, 0x040C, 0x040B, 0x040F,
      0x0452, 0x2018, 0x2019, 0x201C, 0x201D, 0x2022, 0x2013, 0x2014,
      0x007F, 0x2122, 0x0459, 0x203A, 0x045A, 0x045C, 0x045B, 0x045F,
      0x00A0, 0x040E, 0x045E, 0x0408, 0x00A4, 0x0490, 0x00A6, 0x00A7,
      0x0401, 0x00A9, 0x0404, 0x00AB, 0x00AC, 0x00AD, 0x00AE, 0x0407,
      0x00B0, 0x00B1, 0x0406, 0x0456, 0x0491, 0x00B5, 0x00B6, 0x00B7,
      0x0451, 0x2116, 0x0454, 0x00BB, 0x0458, 0x0405, 0x0455, 0x0457};

    inline bool isUnicodeNonCharacter(uint ucs4) {
        // Unicode has a couple of "non-characters" that one can use internally,
        // but are not allowed to be used for text interchange.
        //
        // Those are the last two entries each Unicode Plane (U+FFFE, U+FFFF,
        // U+1FFFE, U+1FFFF, etc.) as well as the entries between U+FDD0 and
        // U+FDEF (inclusive)

        return (ucs4 & 0xfffe) == 0xfffe || (ucs4 - 0xfdd0U) < 16;
    }

    inline CharsetType toCharsetType(const QString & ch_name) {
        if (ch_name == QStringLiteral("utf-8"))
            return utf8;
        else if (ch_name == QStringLiteral("windows-1251"))
            return cp1251;

        return unknown;
    }

    inline void toUtf8(CharsetType charset, QIODevice * io, QString & result, char & in) {
        switch(charset) {
            case utf8: { scanUtf8Char(io, result, in); break;}
            case cp1251: { scanRuChar(io, result, in); break;}
            default: result.append(in);
        }
    }

    inline void scanRuChar(QIODevice * /*io*/, QString & result, char & in) {
        int uc = in < 0 ? in + 256 : in;

        if (uc < 192)
            uc = cp1251_table[uc + 896];
        else
            uc = uc + 848;

        result.append(QChar(uc));
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

    inline void scanUtf8Char(char * io, QString & result) { // not worked
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
