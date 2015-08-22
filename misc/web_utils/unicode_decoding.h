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

    const int cp1251_table[128] = {
            0x82D0,0x83D0,0x9A80E2,0x93D1,0x9E80E2,0xA680E2,0xA080E2,0xA180E2,
            0xAC82E2,0xB080E2,0x89D0,0xB980E2,0x8AD0,0x8CD0,0x8BD0,0x8FD0,
            0x92D1,0x9880E2,0x9980E2,0x9C80E2,0x9D80E2,0xA280E2,0x9380E2,0x9480E2,
            0,0xA284E2,0x99D1,0xBA80E2,0x9AD1,0x9CD1,0x9BD1,0x9FD1,
            0xA0C2,0x8ED0,0x9ED1,0x88D0,0xA4C2,0x90D2,0xA6C2,0xA7C2,
            0x81D0,0xA9C2,0x84D0,0xABC2,0xACC2,0xADC2,0xAEC2,0x87D0,
            0xB0C2,0xB1C2,0x86D0,0x96D1,0x91D2,0xB5C2,0xB6C2,0xB7C2,
            0x91D1,0x9684E2,0x94D1,0xBBC2,0x98D1,0x85D0,0x95D1,0x97D1,
            0x90D0,0x91D0,0x92D0,0x93D0,0x94D0,0x95D0,0x96D0,0x97D0,
            0x98D0,0x99D0,0x9AD0,0x9BD0,0x9CD0,0x9DD0,0x9ED0,0x9FD0,
            0xA0D0,0xA1D0,0xA2D0,0xA3D0,0xA4D0,0xA5D0,0xA6D0,0xA7D0,
            0xA8D0,0xA9D0,0xAAD0,0xABD0,0xACD0,0xADD0,0xAED0,0xAFD0,
            0xB0D0,0xB1D0,0xB2D0,0xB3D0,0xB4D0,0xB5D0,0xB6D0,0xB7D0,
            0xB8D0,0xB9D0,0xBAD0,0xBBD0,0xBCD0,0xBDD0,0xBED0,0xBFD0,
            0x80D1,0x81D1,0x82D1,0x83D1,0x84D1,0x85D1,0x86D1,0x87D1,
            0x88D1,0x89D1,0x8AD1,0x8BD1,0x8CD1,0x8DD1,0x8ED1,0x8FD1
        };

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
        int uc = cp1251_table[(int)(0x7f & in)];

        if (QChar::requiresSurrogates(uc)) { result.append(QChar::highSurrogate(uc)); result.append(QChar::lowSurrogate(uc)); }
        else result.append(QChar(uc));


//        if (in & 0x80) {
//            int v = table[(int)(0x7f & *in++)];
//            if (!v)
//                continue;
//            *out++ = (char)v;
//            *out++ = (char)(v >> 8);
//            if (v >>= 16)
//                *out++ = (char)v;
//        }
//        else
//            *out++ = *in++;
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
