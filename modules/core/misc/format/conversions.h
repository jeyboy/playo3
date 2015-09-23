#ifndef CONVERSIONS_H
#define CONVERSIONS_H

#include <QtCore>

class Conversion {
public:
//    static inline QString timeToStr(qint64 time) {
//        return QString(QStringLiteral("%1:%2")).arg(time / 60).arg(time % 60, 2, 10, QChar('0'));
//    }

    static inline QString paddedNumber(qint64 time) {
        QString res = QString::number(time);
        int len = res.length();

        if (len > 3)
            for (int pos = len - 3; pos > 0; pos -= 3)
                res = res.insert(pos, ' ');

        return res;
    }
};

#endif // CONVERSIONS_H
